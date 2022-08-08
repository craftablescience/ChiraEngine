#pragma once

#include "MaterialUntextured.h"

namespace chira {

class Frame;

class MaterialFramebuffer final : public IMaterial {
public:
    MaterialFramebuffer(std::string identifier_, Frame* frame_)
        : IMaterial(std::move(identifier_))
        , frame(frame_) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
protected:
    Frame* frame;
};

} // namespace chira
