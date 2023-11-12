#pragma once

#include "Resource.h"

namespace chira {

class StringResource : public Resource {
public:
    explicit StringResource(std::string identifier_) : Resource(std::move(identifier_)) {}
    void compile(const std::byte buffer[], std::size_t bufferLength) override;
    [[nodiscard]] const std::string& getString() const;
protected:
    std::string data;
};

} // namespace chira
