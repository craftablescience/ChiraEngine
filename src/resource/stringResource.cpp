#include "stringResource.h"

#include <regex>

using namespace chira;

void stringResource::compile(unsigned char* buffer, std::size_t bufferLength) {
    this->data = std::string{reinterpret_cast<const char*>(buffer), bufferLength};
    this->data = std::regex_replace(this->data.data(), std::regex{"\r\n"}, "\n");
}

const std::string& stringResource::getString() const {
    return this->data;
}
