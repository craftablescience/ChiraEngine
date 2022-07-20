#pragma once

#include <imgui.h>
#include "PropertiesResource.h"

namespace chira {

/// IMPORTANT: All font resources MUST be created inside engine::init() \n
/// Using resource::precacheResource in your init function is recommended \n
/// Before this function, the font list is purged \n
/// After this function, the fonts will be baked, and there will be cake
class FontResource : public PropertiesResource {
public:
    explicit FontResource(const std::string& identifier_) : PropertiesResource(identifier_) {}
    void compile(const nlohmann::json& properties) override;
    [[nodiscard]] ImFont* getFont() const;
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] float getSize() const;
private:
    std::string name;
    float size = 0.0f;
    const ImWchar* range = nullptr;
    ImFont* font = nullptr;
    static const ImWchar* getRangeFromString(std::string_view input);
};

} // namespace chira
