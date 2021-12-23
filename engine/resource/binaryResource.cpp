#include "binaryResource.h"

using namespace chira;

void binaryResource::compile(const unsigned char buffer[], std::size_t bufferLength) {
    this->bufferLength_ = bufferLength - 1;
    this->buffer_ = new unsigned char[this->bufferLength_];
    memcpy(this->buffer_, buffer, this->bufferLength_);
}

binaryResource::~binaryResource() {
    delete[] this->buffer_;
}

const unsigned char* binaryResource::getBuffer() const noexcept {
    return this->buffer_;
}

std::size_t binaryResource::getBufferLength() const noexcept {
    return this->bufferLength_;
}
