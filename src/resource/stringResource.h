#pragma once

#include "abstractResource.h"

class stringResource : public abstractResource {
public:
    stringResource(const std::string& provider_, const std::string& name_) : abstractResource(provider_, name_) {}
    void compile(std::unique_ptr<unsigned char> buffer, unsigned int bufferLength) override;
    [[nodiscard]] const std::string& getString() const;
private:
    std::string data;
};
