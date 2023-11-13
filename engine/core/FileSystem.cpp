#include "FileSystem.h"

#include <filesystem>

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <physfs.h>

#include "config/Config.h"
#include "debug/Logger.h"
#include "CommandLine.h"

using namespace chira;

CHIRA_CREATE_LOG(FILESYSTEM);

bool FileSystem::init() {
	const auto failure = [] {
		LOG_FILESYSTEM.error("PhysFS failed to initialize! Error: {}", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		return false;
	};

	if (!PHYSFS_init(CommandLine::at(0).data())) {
		return failure();
	}

	// Add search paths, going from highest to lowest priority
	bool foundAssets = false;

	// Load zip or folder with -game
	if (auto gameDirArg = std::string{CommandLine::get("-game")}; !gameDirArg.empty()) {
		if (!PHYSFS_mount(gameDirArg.c_str(), nullptr, 1)) return failure();
		foundAssets = true;
	}

#ifdef CHIRA_DEVELOPER_BUILD
	// Load folder with executable name in dev assets directory
	std::string assetDir = std::string{CHIRA_DEVELOPER_ASSET_DIRECTORY} + '/' + CommandLine::getProgramName().data();
	if (!PHYSFS_mount(assetDir.c_str(), nullptr, 1)) return failure();
	foundAssets = true;
#else
	// Load embedded pack file
	// todo(fs): load embedded pack file

	// Load folder or pack file with executable name
	if (auto gameDirRel = std::filesystem::absolute(CommandLine::getProgramName()).string(); std::filesystem::exists(gameDirRel)) {
		if (!PHYSFS_mount(gameDirRel.c_str(), nullptr, 1)) return failure();
		foundAssets = true;
	} else if (auto gameDirZip = gameDirRel + std::string{PACK_FILE_EXTENSION}; std::filesystem::exists(gameDirZip)) {
		if (!PHYSFS_mount(gameDirZip.c_str(), nullptr, 1)) return failure();
		foundAssets = true;
	}
#endif

	// Bail if nothing worked
	if (!foundAssets) {
		LOG_FILESYSTEM.error("Unable to find game assets to mount!");
		return false;
	}

#ifdef CHIRA_DEVELOPER_BUILD
	// Mount internal resources if in a dev build
	if (!PHYSFS_mount(CHIRA_DEVELOPER_ASSET_DIRECTORY "/engine", nullptr, 1)) return failure();
#else
	// todo(fs): do we assume that the engine stuff is already mounted?
	#error "todo(fs): bundle/load engine assets from executable"
#endif

	// By this point we can assume PhysFS has mounted the game files, and we can access the project file
	std::string projectFile;
	if (!readString(PROJECT_FILE_NAME, projectFile)) {
		LOG_FILESYSTEM.error("Unable to find project file!");
		return false;
	}
	std::stringstream projectFileStream{projectFile};
	cereal::JSONInputArchive archive{projectFileStream};
	FileSystem::projectConfig.serialize(archive);

	// Setup write directory
	std::string writeDir = PHYSFS_getPrefDir(CHIRA_PROJECT_NAME, FileSystem::projectConfig.technicalName.c_str());
	if (!PHYSFS_setWriteDir(writeDir.c_str())) return failure();
	if (!PHYSFS_mount(writeDir.c_str(), nullptr, 1)) return failure();

	// All done!
	return true;
}

const FileSystem::ProjectConfig& FileSystem::getProjectConfig() {
	return FileSystem::projectConfig;
}

bool FileSystem::exists(std::string_view path) {
	return PHYSFS_exists(path.data());
}

bool FileSystem::readBytes(std::string_view path, std::vector<std::byte>& data) {
	auto* file = PHYSFS_openRead(path.data());
	if (!file) {
		return false;
	}
	auto len = PHYSFS_fileLength(file);
	data.clear();
	data.resize(len);
	PHYSFS_readBytes(file, data.data(), len);
	return true;
}

bool FileSystem::readString(std::string_view path, std::string& data) {
	std::vector<std::byte> raw;
	if (!readBytes(path, raw)) {
		return false;
	}
	data = std::string{reinterpret_cast<const char*>(raw.data()), raw.size()};
	return true;
}

void FileSystem::deinit() {
	PHYSFS_deinit();
}
