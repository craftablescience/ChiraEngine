#pragma once

#include <imgui.h>
#include <resource/PropertiesResource.h>

namespace chira {

/// IMPORTANT: All font resources MUST be created before Engine::init() is called!
/// Using Resource::precacheResource before init is recommended.1
/// After this function, the fonts will be baked, and there will be cake.
class Font : public PropertiesResource {
public:
    explicit Font(const std::string& identifier_) : PropertiesResource(identifier_) {}
    void compile(const nlohmann::json& properties) override;
    [[nodiscard]] ImFont* getFont() const;
    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] float getSize() const;
private:
    std::string name{"Noto Sans JP"};
    float size = 22.f;
    const ImWchar* range = nullptr;
    std::string rangeStr{"english"};
    ImFont* font = nullptr;
    std::string fontPath{"file://fonts/noto_sans_jp/NotoSansJP-Regular.otf"};
    static const ImWchar* getRangeFromString(std::string_view input);
public:
    CHIRA_PROPS(
            CHIRA_PROP(Font, name),
            CHIRA_PROP(Font, size),
            CHIRA_PROP_NAMED(Font, rangeStr, range),
            CHIRA_PROP_NAMED(Font, fontPath, font)
    );
};

} // namespace chira
