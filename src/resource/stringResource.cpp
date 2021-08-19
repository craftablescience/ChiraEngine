#include "stringResource.h"

void stringResource::compile(std::unique_ptr<unsigned char> buffer, unsigned int bufferLength) {
    this->data = std::string{reinterpret_cast<const char*>(buffer.get()), bufferLength};
}

const std::string& stringResource::getString() const {
    return this->data;
}
