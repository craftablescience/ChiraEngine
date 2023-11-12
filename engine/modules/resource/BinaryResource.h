#pragma once

#include "Resource.h"

namespace chira {

class BinaryResource : public Resource {
public:
    explicit BinaryResource(std::string identifier_) : Resource(std::move(identifier_)) {}
    void compile(const std::byte buffer[], std::size_t bufferLength) override;
    ~BinaryResource() override;
    [[nodiscard]] const std::byte* getBuffer() const;
    [[nodiscard]] std::size_t getBufferLength() const;
protected:
    std::byte* buffer_ = nullptr;
    std::size_t bufferLength_ = 0;
};

} // namespace chira
