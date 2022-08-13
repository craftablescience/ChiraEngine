#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glad/gl.h>
#include <math/Types.h>
#include <utility/HandleObject.h>

namespace chira {

class Shader;

class UniformBufferObject : public HandleObject<unsigned int> {
public:
    explicit UniformBufferObject(std::string name_, unsigned int size);
    [[nodiscard]] unsigned int getBindingPoint() const;
    void update(const byte buffer[], GLsizeiptr length) const;
    void bindToShader(Shader* shader_) const;
protected:
    unsigned int bindingPoint = 0;
    std::string name;
    static unsigned int uboBindingPoint;
};

/// Stores two mat4 values, named PV
class UBO_PerspectiveView : public UniformBufferObject {
public:
    static UBO_PerspectiveView* get();
    void update(const glm::mat4& proj, const glm::mat4& view) const;
private:
    using UniformBufferObject::update;
    UBO_PerspectiveView();
    static std::unique_ptr<UBO_PerspectiveView> singleton;
};

} // namespace chira
