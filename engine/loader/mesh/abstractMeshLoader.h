#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <utility/math/vertex.h>
#include <utility/platform.h>

namespace chira {
    struct AbstractMeshLoader {
        virtual ~AbstractMeshLoader() = default;
        virtual void loadMesh(const std::string& identifier, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) const = 0;
        [[nodiscard]] virtual std::vector<byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) const = 0;
        static void addMeshLoader(const std::string& name, AbstractMeshLoader* meshLoader);
        static AbstractMeshLoader* getMeshLoader(const std::string& name);
    private:
        static inline std::unordered_map<std::string, std::unique_ptr<AbstractMeshLoader>> meshLoaders;
    };
}
