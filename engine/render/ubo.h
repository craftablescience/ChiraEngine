#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glad/gl.h>
#include <utility/handleObject.h>

namespace chira {
    class Shader;

    class UBO : public HandleObject<unsigned int> {
    public:
        explicit UBO(std::string name_, unsigned int size);
        [[nodiscard]] unsigned int getBindingPoint() const;
        void update(const unsigned char buffer[], GLsizeiptr length) const;
        void bindToShader(Shader* shader_) const;
    protected:
        unsigned int bindingPoint = 0;
        std::string name;
        static unsigned int uboBindingPoint;
    };

    /// Stores two mat4 values, named PV
    class UBO_PV : public UBO {
    public:
        static UBO_PV* get();
        void update(const glm::mat4& proj, const glm::mat4& view) const;
    private:
        using UBO::update;
        explicit UBO_PV(const std::string& name);
        static std::unique_ptr<UBO_PV> singleton;
    };
}
