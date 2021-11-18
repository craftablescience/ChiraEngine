#pragma once

namespace chira {
    enum class sharedPointerCastType {
        STATIC_CAST,
        DYNAMIC_CAST,
        REINTERPRET_CAST,
        C_CAST
    };

    template<typename T>
    class sharedPointer {
    public:
        sharedPointer() = default;
        explicit sharedPointer(T* inputPtr) : ptr(inputPtr), refCount(new unsigned int(1)) {}
        sharedPointer(T* inputPtr, unsigned int* refCountPtr) : ptr(inputPtr), refCount(refCountPtr) {
            (*this->refCount)++;
        }
        sharedPointer<T>& operator=(const sharedPointer<T>& other) {
            if (this != &other) {
                if ((this->refCount) && ((*this->refCount) <= 1)) {
                    delete this->ptr;
                    delete this->refCount;
                }
                this->ptr = other.ptr;
                this->refCount = other.refCount;
                (*this->refCount)++;
            }
            return *this;
        }
        sharedPointer(const sharedPointer<T>& other) {
            if (this != &other) {
                if ((this->refCount) && ((*this->refCount) <= 1)) {
                    delete this->ptr;
                    delete this->refCount;
                }
                this->ptr = other.ptr;
                this->refCount = other.refCount;
                (*this->refCount)++;
            }
        }
        sharedPointer<T>& operator=(sharedPointer<T>&& other) noexcept {
            if ((this->refCount) && ((*this->refCount) <= 1)) {
                delete this->ptr;
                delete this->refCount;
            }
            this->ptr = other.ptr;
            this->refCount = other.refCount;
            other.ptr = nullptr;
            other.refCount = nullptr;
            return *this;
        }
        sharedPointer(sharedPointer<T>&& other) noexcept {
            *this = std::move(other);
        }
        ~sharedPointer() {
            if (!this->refCount) {
                delete this->ptr;
                return;
            }
            (*this->refCount)--;
            if ((*this->refCount) <= 0) {
                delete this->ptr;
                delete this->refCount;
            }
        }
        T* get() const noexcept {
            return this->ptr;
        }
        T& operator*() const noexcept {
            return *this->ptr;
        }
        T* operator->() const noexcept {
            return this->ptr;
        }
        bool operator!() const noexcept {
            return !bool(this->ptr);
        }
        explicit operator bool() const noexcept {
            return bool(this->ptr);
        }
        [[nodiscard]] unsigned int useCount() {
            return *this->refCount;
        }
        template<typename U>
        sharedPointer<U> castStatic() {
            return sharedPointer<U>(static_cast<U*>(this->ptr), this->refCount);
        }
        template<typename U>
        sharedPointer<U> castDynamic() {
            return sharedPointer<U>(dynamic_cast<U*>(this->ptr), this->refCount);
        }
        template<typename U>
        sharedPointer<U> castReinterpret() {
            return sharedPointer<U>(reinterpret_cast<U*>(this->ptr), this->refCount);
        }
        template<typename U>
        sharedPointer<U> cast(const sharedPointerCastType& type) {
            switch (type) {
                case sharedPointerCastType::STATIC_CAST:
                    return this->castStatic<U>();
                case sharedPointerCastType::DYNAMIC_CAST:
                    return this->castDynamic<U>();
                case sharedPointerCastType::REINTERPRET_CAST:
                    return this->castReinterpret<U>();
                case sharedPointerCastType::C_CAST:
                    return sharedPointer<U>((U*)(this->ptr), this->refCount);
            }
        }
    private:
        T* ptr = nullptr;
        unsigned int* refCount = nullptr;
    };
}
