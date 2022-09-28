#pragma once

#include <soloud.h>

namespace chira {

class AudioEngine {
    friend class Engine;
protected:
    AudioEngine();
    SoLoud::Soloud& get();
public:
    ~AudioEngine();
private:
    SoLoud::Soloud engine{};
};

} // namespace chira
