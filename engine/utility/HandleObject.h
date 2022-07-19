#pragma once

namespace chira {

template<typename T> class HandleObject {
public:
    explicit HandleObject(T defaultValue = -1) : handle(defaultValue) {}
    virtual ~HandleObject() = default;
    [[nodiscard]] T getHandle() const {
        return this->handle;
    }
protected:
    T handle;
};

}
