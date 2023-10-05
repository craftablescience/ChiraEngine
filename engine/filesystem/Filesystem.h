#pragma once

#include <string>
#include <vector>

#include <cereal/cereal.hpp>

namespace chira::Filesystem {

struct ProjectInfo {
    std::string name;
    std::string version;
    bool splashscreen;
    std::vector<std::string> authors;

    template<typename Archive>
    void serialize(Archive& archive) {
        archive(
            CEREAL_NVP(name),
            CEREAL_NVP(version),
            CEREAL_NVP(splashscreen),
            CEREAL_NVP(authors)
        );
    }
};

const ProjectInfo& getProjectInfo();

bool init();

} // namespace chira::Filesystem
