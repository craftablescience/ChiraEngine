#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glad/gl.h>
#include <utility/handleObject.h>

namespace chira {
    class shader;

    class ubo : public handleObject<unsigned int> {
    public:
        explicit ubo(std::string name_, unsigned int size);
        [[nodiscard]] unsigned int getBindingPoint() const;
        void update(const unsigned char buffer[], GLsizeiptr length);
        void bindToShader(shader* shader_) const;
    protected:
        unsigned int bindingPoint = 0;
        std::string name;
        static inline unsigned int uboBindingPoint = 0;
    };

    /// Stores two mat4 values, named PV
    class uboPV : public ubo {
    public:
        static uboPV* get();
        void update(const glm::mat4& proj, const glm::mat4& view);
    private:
        using ubo::update;
        explicit uboPV(const std::string& name);
        static std::unique_ptr<uboPV> singleton;
    };
}
