#include "BinaryResource.h"

#include <cstring>

using namespace chira;

void BinaryResource::compile(const std::byte buffer[], std::size_t bufferLength) {
    this->bufferLength_ = bufferLength - 1;
    this->buffer_ = new std::byte[this->bufferLength_];
    std::memcpy(this->buffer_, buffer, this->bufferLength_);
}

BinaryResource::~BinaryResource() {
    delete[] this->buffer_;
}

const std::byte* BinaryResource::getBuffer() const {
    return this->buffer_;
}

std::byte* BinaryResource::getBuffer() {
	return this->buffer_;
}

std::size_t BinaryResource::getBufferLength() const {
    return this->bufferLength_;
}
