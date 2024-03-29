#include "StringResource.h"

#include <utility/String.h>

using namespace chira;

void StringResource::compile(const byte buffer[], std::size_t bufferLength) {
    this->data = {reinterpret_cast<const char*>(buffer), bufferLength};
    String::replace(this->data, "\r\n", "\n");
}

const std::string& StringResource::getString() const {
    return this->data;
}
