#pragma once

#include "MaterialFactory.h"

namespace chira {

class MaterialUntextured final : public IMaterial {
public:
    explicit MaterialUntextured(std::string identifier_) : IMaterial(std::move(identifier_)) {}
    void compile(const byte buffer[], std::size_t bufferLength) override;

public:
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("shader", this->shaderPath)
        );
    }

private:
    CHIRA_REGISTER_MATERIAL_TYPE(MaterialUntextured);
};

} // namespace chira
