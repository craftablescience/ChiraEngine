#pragma once

namespace chira {
    class AbstractImage {
    public:
        [[nodiscard]] virtual unsigned char* getData() {
            return this->data;
        }
        virtual ~AbstractImage() = default;
    protected:
        // The pointer is deleted by subclass(es)
        unsigned char* data = nullptr;
    };
}
