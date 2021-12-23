#pragma once

namespace chira {
    template<typename T>
    class handleObject {
    public:
        [[nodiscard]] T getHandle() const {
            return this->handle;
        }
    protected:
        T handle = -1;
    };
}
