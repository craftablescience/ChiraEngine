#pragma once

#include <vector>
#include <string>
#include <glad/gl.h>
#include <loader/mesh/IMeshLoader.h>
#include <render/material/MaterialFactory.h>

namespace chira {

class MeshData {
public:
    MeshData() = default;
    void render(glm::mat4 model);
    virtual ~MeshData();
    [[nodiscard]] SharedPointer<MaterialBase> getMaterial() const;
    void setMaterial(SharedPointer<MaterialBase> newMaterial);
    [[nodiscard]] std::vector<byte> getMeshData(const std::string& meshLoader) const;
    void appendMeshData(const std::string& loader, const std::string& identifier);
protected:
    bool initialized = false;
    int drawMode = GL_STATIC_DRAW;
    int depthFunction = GL_LEQUAL;
    bool backfaceCulling = true;
    int cullType = GL_BACK;
    SharedPointer<MaterialBase> material;
    unsigned int vboHandle = 0, vaoHandle = 0, eboHandle = 0;
    std::vector<Vertex> vertices{};
    std::vector<unsigned int> indices{};
    /// Establishes the OpenGL buffers and copies the current mesh data into them.
    void setupForRendering();
    /// Updates the OpenGL buffers with the current mesh data.
    void updateMeshData();
    /// Does not call updateMeshData().
    void clearMeshData();
};

} // namespace chira
