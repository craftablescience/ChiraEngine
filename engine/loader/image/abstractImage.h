#pragma once

#include <utility/platform.h>

namespace chira {
    class AbstractImage {
    public:
        [[nodiscard]] virtual unsigned char* getData() const {
            return this->data;
        }
        virtual ~AbstractImage() = default;
    protected:
        // The pointer is deleted by subclass(es)
        byte* data = nullptr;
    };
}
