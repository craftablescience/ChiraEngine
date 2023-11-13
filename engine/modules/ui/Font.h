#pragma once

#include <imgui.h>

#include <core/utility/Serial.h>
#include <resource/BinaryResource.h>

namespace chira {

/// IMPORTANT: All font resources MUST be created before Device::createWindow is called!
/// After this, the fonts will be baked, and there will be cake.
class Font : public Resource {
public:
    explicit Font(const std::string& path_) : Resource(path_) {}

    void compile(const std::byte buffer[], std::size_t bufferLength) override;

    [[nodiscard]] ImFont* getFont() const;

    [[nodiscard]] const std::string& getName() const;

    [[nodiscard]] float getSize() const;

private:
    std::string name{"Noto Sans JP"};
    float size = 22.f;
    const ImWchar* range = nullptr;
    std::string rangeStr{"english"};
    ImFont* font = nullptr;
    std::string fontPath{"fonts/noto_sans_jp/NotoSansJP-Regular.otf"};

	SharedPointer<BinaryResource> fontData{};

    static const ImWchar* getRangeFromString(std::string_view input);

public:
    template<typename Archive>
    void serialize(Archive& ar) {
        ar(
                cereal::make_nvp("name", this->name),
                cereal::make_nvp("size", this->size),
                cereal::make_nvp("range", this->rangeStr),
                cereal::make_nvp("font", this->fontPath)
        );
    }
};

} // namespace chira
