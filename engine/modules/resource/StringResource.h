#pragma once

#include "Resource.h"

namespace chira {

class StringResource : public Resource {
public:
    explicit StringResource(std::string path_) : Resource(std::move(path_)) {}
    void compile(const std::byte buffer[], std::size_t bufferLength) override;
    [[nodiscard]] const std::string& getString() const;
protected:
    std::string data;
};

} // namespace chira
