#include "AudioEngine.h"

using namespace chira;

AudioEngine::AudioEngine() {
    this->engine.init();
}

AudioEngine::~AudioEngine() {
    this->engine.deinit();
}

SoLoud::Soloud& AudioEngine::get() {
    return this->engine;
}
