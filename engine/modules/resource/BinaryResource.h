#pragma once

#include "Resource.h"

namespace chira {

class BinaryResource : public Resource {
public:
    explicit BinaryResource(std::string path_) : Resource(std::move(path_)) {}

	~BinaryResource() override;

    void compile(const std::byte buffer[], std::size_t bufferLength) override;

    [[nodiscard]] const std::byte* getBuffer() const;

    [[nodiscard]] std::byte* getBuffer();

    [[nodiscard]] std::size_t getBufferLength() const;

protected:
    std::byte* buffer_ = nullptr;
    std::size_t bufferLength_ = 0;
};

} // namespace chira
