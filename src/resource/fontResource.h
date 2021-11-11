#pragma once

#include "propertiesResource.h"
#include "imgui.h"

namespace chira {
    /// IMPORTANT: All font resources MUST be created inside engine::init() \n
    /// Using resourceManager::precacheResource in your init function is recommended \n
    /// Before this function, the font list is purged \n
    /// After this function, the fonts will be baked, and there will be cake
    class fontResource : public propertiesResource {
    public:
        explicit fontResource(const std::string& identifier_) : propertiesResource(identifier_) {}
        void compile(const nlohmann::json& properties) override;
        [[nodiscard]] ImFont* getFont() const;
        [[nodiscard]] const std::string& getName() const;
        [[nodiscard]] float getSize() const;
    private:
        std::string name;
        float size = 0.0f;
        const ImWchar* range = nullptr;
        ImFont* font = nullptr;
        static const ImWchar* getRangeFromString(const std::string& input);
    };
}
