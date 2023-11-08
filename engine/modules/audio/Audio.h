#pragma once

#include <soloud.h>

#include <core/IModule.h>

namespace chira {

CHIRA_CREATE_MODULE(Audio) {
    CHIRA_CREATE_MODULE_DEPS();

    bool preinit() override;

    void deinit() override {
        this->soundEngine.deinit();
    }

    [[nodiscard]] SoLoud::Soloud& get() {
        // Assume we've initialized it
        return soundEngine;
    }

private:
    SoLoud::Soloud soundEngine;
};

} // namespace chira
