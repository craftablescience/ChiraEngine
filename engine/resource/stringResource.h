#pragma once

#include "resource.h"

namespace chira {
    class StringResource : public Resource {
    public:
        explicit StringResource(const std::string& identifier_) : Resource(identifier_) {}
        void compile(const unsigned char buffer[], std::size_t bufferLength) override;
        [[nodiscard]] const std::string& getString() const;
    private:
        std::string data;
    };
}
