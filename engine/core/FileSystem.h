#pragma once

#include <string>
#include <string_view>
#include <vector>

#include <cereal/cereal.hpp>

#include "utility/NoCopyOrMove.h"

namespace chira {

struct FileSystem : public NoCopyOrMove {
	struct ProjectConfig {
		std::string technicalName;
		std::string version;
		std::vector<std::string> authors;

		bool splashscreen;

		template<typename Archive>
		void serialize(Archive& archive) {
			archive(
					CEREAL_NVP(technicalName),
					CEREAL_NVP(version),
					CEREAL_NVP(authors),
					CEREAL_NVP(splashscreen)
			);
		}
	};

	/// Initialization is only meant to happen once per program instantiation.
	[[nodiscard]] static bool init();

	[[nodiscard]] static const ProjectConfig& getProjectConfig();

	[[nodiscard]] static bool exists(std::string_view path);

	[[nodiscard]] static bool readBytes(std::string_view path, std::vector<std::byte>& data);

	[[nodiscard]] static bool readString(std::string_view path, std::string& data);

	static void deinit();

	static constexpr std::string_view PACK_FILE_EXTENSION = ".cpck";
	static constexpr std::string_view PROJECT_FILE_NAME = "project.chira";

private:
	static inline ProjectConfig projectConfig{};
};

} // namespace chira
