#pragma once

#include "resource.h"

namespace chira {
    class BinaryResource : public Resource {
    public:
        explicit BinaryResource(const std::string& identifier_) : Resource(identifier_) {}
        void compile(const unsigned char buffer[], std::size_t bufferLength) override;
        ~BinaryResource() override;
        [[nodiscard]] const unsigned char* getBuffer() const noexcept;
        [[nodiscard]] std::size_t getBufferLength() const noexcept;
    private:
        unsigned char* buffer_ = nullptr;
        std::size_t bufferLength_ = 0;
    };
}
