#pragma once

class compilable {
public:
    virtual void compile() = 0;
    virtual void discard() = 0;
};
