#pragma once

#include "Resource.h"

namespace chira {

class BinaryResource : public Resource {
public:
    explicit BinaryResource(std::string identifier_) : Resource(std::move(identifier_)) {}
    void compile(const unsigned char buffer[], std::size_t bufferLength) override;
    ~BinaryResource() override;
    [[nodiscard]] const unsigned char* getBuffer() const;
    [[nodiscard]] std::size_t getBufferLength() const;
protected:
    unsigned char* buffer_ = nullptr;
    std::size_t bufferLength_ = 0;
};

}
