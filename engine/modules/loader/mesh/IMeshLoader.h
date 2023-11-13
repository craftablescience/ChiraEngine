#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include <core/math/Vertex.h>
#include <core/Platform.h>

namespace chira {

class IMeshLoader {
public:
    virtual ~IMeshLoader() = default;

    virtual void loadMesh(const std::string& path, std::vector<Vertex>& vertices, std::vector<Index>& indices) const = 0;

    [[nodiscard]] virtual std::vector<std::byte> createMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) const = 0;

    static void addMeshLoader(const std::string& name, IMeshLoader* meshLoader);

    static IMeshLoader* getMeshLoader(const std::string& name);

private:
    static inline std::unordered_map<std::string, std::unique_ptr<IMeshLoader>> meshLoaders;
};

} // namespace chira
