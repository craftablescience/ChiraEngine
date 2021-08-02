#pragma once

class abstractImage {
public:
    virtual ~abstractImage() {
        delete this->data;
    }
    [[nodiscard]] virtual unsigned char* getData() {
        return this->data;
    }
protected:
    unsigned char* data = nullptr;
};
