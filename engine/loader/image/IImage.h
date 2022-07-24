#pragma once

#include <math/Types.h>

namespace chira {

// haha funny name
class IImage {
public:
    [[nodiscard]] virtual unsigned char* getData() const {
        return this->data;
    }
    virtual ~IImage() = default;
protected:
    // The pointer is deleted by subclass(es)
    byte* data = nullptr;
};

} // namespace chira
