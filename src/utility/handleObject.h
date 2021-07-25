#pragma once

#include "compilable.h"

class handleObject : public compilable {
public:
    [[nodiscard]] int getHandle() const;
protected:
    int handle = -1;
};
