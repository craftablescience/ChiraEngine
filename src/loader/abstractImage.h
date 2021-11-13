#pragma once

namespace chira {
    class abstractImage {
    public:
        [[nodiscard]] virtual unsigned char* getData() {
            return this->data;
        }
        virtual ~abstractImage() = default;
    protected:
        // The pointer is deleted by subclass(es)
        unsigned char* data = nullptr;
    };
}
