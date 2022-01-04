#pragma once

namespace chira {
    template<typename T>
    class HandleObject {
    public:
        [[nodiscard]] T getHandle() const {
            return this->handle;
        }
    protected:
        T handle = -1;
    };
}
