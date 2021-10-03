#pragma once

namespace chira {
    class handleObject {
    public:
        [[nodiscard]] int getHandle() const {
            return this->handle;
        }
    protected:
        int handle = -1;
    };
}
