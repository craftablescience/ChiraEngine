#include "Filesystem.h"

#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <physfs.h>
#include <config/Config.h>
#include <core/CommandLine.h>
#include <utility/String.h>

using namespace chira;

static Filesystem::ProjectInfo& getMutableProjectInfo() {
    static Filesystem::ProjectInfo projectInfo;
    return projectInfo;
}

const Filesystem::ProjectInfo& Filesystem::getProjectInfo() {
    return getMutableProjectInfo();
}

bool Filesystem::init() {
    // If we're given a project file through the command line, then we're probably loading a raw project directory
    std::string projectDir = CommandLine::getDefaultArgument().data();
    if (projectDir.empty()) {
        projectDir = CommandLine::get("--project-dir");
        projectDir = String::stripRight(projectDir, "\\/");
    }
    if (projectDir.empty()) {
        // No given project file, use hardcoded paths
        projectDir = ".";
    }

    // We need to load the project file with regular filesystem operations, then we can start using physfs.
    std::ifstream file{projectDir + "/project.chira"};
    cereal::JSONInputArchive archive{file};
    auto& projectInfo = getMutableProjectInfo();
    projectInfo.serialize(archive);

    // todo(fs): check for errors
    PHYSFS_init(CommandLine::getProgramName().data());
    PHYSFS_setSaneConfig(CHIRA_PROJECT_NAME, projectInfo.name.c_str(), nullptr, 0, 0);
    PHYSFS_mount(PHYSFS_getWriteDir(), "/persist/", 1);

#ifdef DEBUG
    // Mount the internal resources
    PHYSFS_mount(CHIRA_DEBUG_ASSET_DIR "/engine", nullptr, 0);
    PHYSFS_mount(CHIRA_DEBUG_ASSET_DIR "/editor", nullptr, 0);
#endif
    PHYSFS_mount(projectDir.data(), nullptr, 1);
    return true;
}
