#pragma once

#include "MaterialUntextured.h"

namespace chira {

class Frame;

class MaterialFrameBuffer final : public IMaterial {
public:
    MaterialFrameBuffer(std::string identifier_, Renderer::FrameBufferHandle* handle_)
        : IMaterial(std::move(identifier_))
        , handle(handle_) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
protected:
    Renderer::FrameBufferHandle* handle;
public:
    CHIRA_PROPS_INHERITED(IMaterial) ();
};

} // namespace chira
