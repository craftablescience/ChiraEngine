#pragma once

#include "MaterialUntextured.h"

namespace chira {

class Frame;

class MaterialFramebuffer : public MaterialUntextured {
public:
    MaterialFramebuffer(const std::string& identifier_, Frame* frame_)
        : MaterialUntextured(identifier_)
        , frame(frame_) {}
    void compile(const nlohmann::json& properties) override;
    void use() const override;
protected:
    Frame* frame;
};

}
