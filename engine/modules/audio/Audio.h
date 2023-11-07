#pragma once

#include <soloud.h>

#include <core/IModule.h>

namespace chira {

CHIRA_CREATE_MODULE(Audio) {
    static inline const std::vector<std::string_view> DEPS;

    void init() override {
        if (this->initialized)
            return;
        this->soundEngine.init();
        this->initialized = true;
    }

    void deinit() override {
        this->soundEngine.deinit();
    }

    [[nodiscard]] SoLoud::Soloud& get() {
        if (!this->initialized) {
            // This is bad, but whatever
            this->init();
        }
        return soundEngine;
    }

private:
    SoLoud::Soloud soundEngine;
};

} // namespace chira
