#pragma once

class abstractImage {
public:
    [[nodiscard]] virtual unsigned char* getData() {
        return this->data;
    }
protected:
    unsigned char* data = nullptr;
};
