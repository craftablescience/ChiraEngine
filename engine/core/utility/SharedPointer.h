#pragma once

#include "../debug/Assertions.h"

namespace chira {

enum class CastType {
    STATIC_CAST,
    DYNAMIC_CAST,
    REINTERPRET_CAST,
    ASSERT_CAST,
    C_CAST
};

struct SharedPointerMetadata {
    unsigned int refCount = 1;
    /// If the refcount is less than or equal to this number in the destructor (after it is subtracted once),
    /// this is the last holder of the pointer and sharedPointer::ptr will be deleted.\n
    /// 1 is the regular value, because this class is intended to be used as a resource, and the resource
    /// manager always holds 1 copy of the pointer.
    unsigned int holderAmountForDelete = 1;
    SharedPointerMetadata() = default;
    explicit SharedPointerMetadata(unsigned int refCount_) : refCount(refCount_) {}
    SharedPointerMetadata(unsigned int refCount_, unsigned int holderAmountForDelete_) : refCount(refCount_), holderAmountForDelete(holderAmountForDelete_) {}
    SharedPointerMetadata(const SharedPointerMetadata& other) = delete;
    SharedPointerMetadata& operator=(const SharedPointerMetadata& other) = delete;
    SharedPointerMetadata(SharedPointerMetadata&& other) = delete;
    SharedPointerMetadata& operator=(SharedPointerMetadata&& other) = delete;
};

template<typename T> class SharedPointer {
public:
    SharedPointer() = default;
    explicit SharedPointer(T* inputPtr) : ptr(inputPtr), data(new SharedPointerMetadata{1}) {}
    SharedPointer(T* inputPtr, SharedPointerMetadata* data_) : ptr(inputPtr), data(data_) {
        if (this->data) {
            this->data->refCount++;
        }
    }
    SharedPointer<T>& operator=(const SharedPointer<T>& other) noexcept {
        if (this == &other)
            return *this;

        if ((this->data) && (this->data->refCount <= this->data->holderAmountForDelete)) {
            delete this->ptr;
            delete this->data;
        }
        this->ptr = other.ptr;
        this->data = other.data;
        if (this->data) {
            this->data->refCount++;
        }
        return *this;
    }
    SharedPointer(const SharedPointer<T>& other) noexcept {
        if (this == &other)
            return;

        if ((this->data) && (this->data->refCount <= this->data->holderAmountForDelete)) {
            delete this->ptr;
            delete this->data;
        }
        this->ptr = other.ptr;
        this->data = other.data;
        if (this->data) {
            this->data->refCount++;
        }
    }
    SharedPointer<T>& operator=(SharedPointer<T>&& other) noexcept {
        if ((this->data) && (this->data->refCount <= this->data->holderAmountForDelete)) {
            delete this->ptr;
            delete this->data;
        }
        this->ptr = other.ptr;
        this->data = other.data;
        other.ptr = nullptr;
        other.data = nullptr;
        return *this;
    }
    SharedPointer(SharedPointer<T>&& other) noexcept {
        *this = std::move(other);
    }
    ~SharedPointer() {
        if (!this->data) {
            delete this->ptr;
            return;
        }
        this->data->refCount--;
        if (this->data->refCount == this->data->holderAmountForDelete) {
            delete this->ptr;
        }
        if (this->data->refCount == 0) {
            delete this->data;
        }
    }
    T* get() const noexcept {
        return this->ptr;
    }
    T& operator*() const {
        return *this->ptr;
    }
    T* operator->() const noexcept {
        return this->ptr;
    }
    explicit operator bool() const {
        return bool(this->ptr);
    }
    bool operator!() const {
        return !(bool(this->ptr));
    }
    [[nodiscard]] unsigned int useCount() const {
        if (this->data)
            return this->data->refCount;
        else
            return 0;
    }
    [[nodiscard]] unsigned int getHolderAmountForDelete() const {
        if (this->data) {
            return this->data->holderAmountForDelete;
        } else {
            return 0;
        }
    }
    void setHolderAmountForDelete(unsigned int newHolderAmountForDelete) const {
        if (this->data) {
            this->data->holderAmountForDelete = newHolderAmountForDelete;
        }
    }
    template<typename U>
    SharedPointer<U> castStatic() const {
        return SharedPointer<U>(static_cast<U*>(this->ptr), this->data);
    }
    template<typename U>
    SharedPointer<U> castDynamic() const {
        return SharedPointer<U>(dynamic_cast<U*>(this->ptr), this->data);
    }
    template<typename U>
    SharedPointer<U> castAssert() const {
        return SharedPointer<U>(assertCast<U*>(this->ptr), this->data);
    }
    template<typename U>
    SharedPointer<U> castReinterpret() const {
        return SharedPointer<U>(reinterpret_cast<U*>(this->ptr), this->data);
    }
    template<typename U>
    SharedPointer<U> cast(CastType type = CastType::ASSERT_CAST) const {
        switch (type) {
            using enum CastType;
            case STATIC_CAST:
                return this->castStatic<U>();
            case DYNAMIC_CAST:
                return this->castDynamic<U>();
            case REINTERPRET_CAST:
                return this->castReinterpret<U>();
            case ASSERT_CAST:
                return this->castAssert<U>();
            case C_CAST:
                return SharedPointer<U>((U*)(this->ptr), this->data);
            CHIRA_NO_DEFAULT;
        }
    }
protected:
    T* ptr = nullptr;
    SharedPointerMetadata* data = nullptr;
};

} // namespace chira
