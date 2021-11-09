#pragma once

#include "abstractResource.h"

namespace chira {
    class stringResource : public abstractResource {
    public:
        explicit stringResource(const std::string& identifier_) : abstractResource(identifier_) {}
        void compile(const unsigned char buffer[], std::size_t bufferLength) override;
        stringResource* copy() override;
        [[nodiscard]] const std::string& getString() const;
    private:
        std::string data;
    };
}
