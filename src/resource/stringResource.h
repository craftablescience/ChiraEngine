#pragma once

#include "resource.h"

namespace chira {
    class stringResource : public resource {
    public:
        explicit stringResource(const std::string& identifier_) : resource(identifier_) {}
        void compile(const unsigned char buffer[], std::size_t bufferLength) override;
        [[nodiscard]] const std::string& getString() const noexcept;
    private:
        std::string data;
    };
}
