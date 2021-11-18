#pragma once

namespace chira {
    enum class pointerCastType {
        STATIC_CAST,
        DYNAMIC_CAST,
        REINTERPRET_CAST,
        C_CAST
    };

    struct sharedPointerMetadata {
        unsigned int refCount = 1;
        /// If the refcount is less than or equal to this number in the destructor (after it is subtracted once),
        /// this is the last holder of the pointer and sharedPointer::ptr will be deleted.\n
        /// 1 is the regular value, because this class is intended to be used as a resource, and the resource
        /// manager always holds 1 copy of the pointer.
        unsigned int holderAmountForDelete = 1;
        sharedPointerMetadata() = default;
        explicit sharedPointerMetadata(unsigned int refCount_) : refCount(refCount_) {}
        sharedPointerMetadata(unsigned int refCount_, unsigned int holderAmountForDelete_) : refCount(refCount_), holderAmountForDelete(holderAmountForDelete_) {}
        sharedPointerMetadata(const sharedPointerMetadata& other) = delete;
        sharedPointerMetadata& operator=(const sharedPointerMetadata& other) = delete;
        sharedPointerMetadata(sharedPointerMetadata&& other) = delete;
        sharedPointerMetadata& operator=(sharedPointerMetadata&& other) = delete;
    };

    template<typename T>
    class sharedPointer {
    public:
        sharedPointer() = default;
        explicit sharedPointer(T* inputPtr) : ptr(inputPtr), data(new sharedPointerMetadata{1}) {}
        sharedPointer(T* inputPtr, sharedPointerMetadata* data_) : ptr(inputPtr), data(data_) {
            if (this->data) {
                this->data->refCount++;
            }
        }
        sharedPointer<T>& operator=(const sharedPointer<T>& other) noexcept {
            if (this != &other) {
                if ((this->data) && (this->data->refCount <= 1)) {
                    delete this->ptr;
                    delete this->data;
                }
                this->ptr = other.ptr;
                this->data = other.data;
                if (this->data) {
                    this->data->refCount++;
                }
            }
            return *this;
        }
        sharedPointer(const sharedPointer<T>& other) noexcept {
            if (this != &other) {
                if ((this->data) && (this->data->refCount <= 1)) {
                    delete this->ptr;
                    delete this->data;
                }
                this->ptr = other.ptr;
                this->data = other.data;
                if (this->data) {
                    this->data->refCount++;
                }
            }
        }
        sharedPointer<T>& operator=(sharedPointer<T>&& other) noexcept {
            if ((this->data) && (this->data->refCount <= 1)) {
                delete this->ptr;
                delete this->data;
            }
            this->ptr = other.ptr;
            this->data = other.data;
            other.ptr = nullptr;
            other.data = nullptr;
            return *this;
        }
        sharedPointer(sharedPointer<T>&& other) noexcept {
            *this = std::move(other);
        }
        ~sharedPointer() {
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
        T& operator*() const noexcept {
            return *this->ptr;
        }
        T* operator->() const noexcept {
            return this->ptr;
        }
        bool operator!() const noexcept {
            return !(bool(this->ptr));
        }
        explicit operator bool() const noexcept {
            return bool(this->ptr);
        }
        [[nodiscard]] unsigned int useCount() const noexcept {
            if (this->data) {
                return this->data->refCount;
            } else {
                return 0;
            }
        }
        [[nodiscard]] unsigned int getHolderAmountForDelete() const noexcept {
            if (this->data) {
                return this->data->holderAmountForDelete;
            } else {
                return 0;
            }
        }
        void setHolderAmountForDelete(unsigned int newHolderAmountForDelete) noexcept {
            if (this->data) {
                this->data->holderAmountForDelete = newHolderAmountForDelete;
            }
        }
        template<typename U>
        sharedPointer<U> castStatic() const {
            return sharedPointer<U>(static_cast<U*>(this->ptr), this->data);
        }
        template<typename U>
        sharedPointer<U> castDynamic() const {
            return sharedPointer<U>(dynamic_cast<U*>(this->ptr), this->data);
        }
        template<typename U>
        sharedPointer<U> castReinterpret() const {
            return sharedPointer<U>(reinterpret_cast<U*>(this->ptr), this->data);
        }
        template<typename U>
        sharedPointer<U> cast(const pointerCastType& type) const {
            switch (type) {
                case pointerCastType::STATIC_CAST:
                    return this->castStatic<U>();
                case pointerCastType::DYNAMIC_CAST:
                    return this->castDynamic<U>();
                case pointerCastType::REINTERPRET_CAST:
                    return this->castReinterpret<U>();
                case pointerCastType::C_CAST:
                    return sharedPointer<U>((U*)(this->ptr), this->data);
            }
        }
    protected:
        T* ptr = nullptr;
        sharedPointerMetadata* data = nullptr;
    };
}
