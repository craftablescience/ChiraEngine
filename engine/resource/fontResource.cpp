#include "fontResource.h"

#include "provider/filesystemResourceProvider.h"

using namespace chira;

void FontResource::compile(const nlohmann::json& properties) {
    ImGuiIO& io = ImGui::GetIO();
    std::string path = FilesystemResourceProvider::getResourceAbsolutePath(getProperty<std::string>(properties["properties"], "font", "file://fonts/noto_sans_jp/NotoSansJP-Regular.otf", true));
    this->name = getProperty<std::string>(properties["properties"], "name", "Noto Sans JP", true);
    this->size = getProperty(properties["properties"], "size", 22.f, true);
    this->range = FontResource::getRangeFromString(getProperty<std::string>(properties["properties"], "range", "english"));
    this->font = io.Fonts->AddFontFromFileTTF(path.c_str(), this->size, nullptr, this->range);
}

ImFont* FontResource::getFont() const {
    return this->font;
}

const std::string& FontResource::getName() const {
    return this->name;
}

float FontResource::getSize() const {
    return this->size;
}

const ImWchar* FontResource::getRangeFromString(const std::string& input) {
    ImGuiIO &io = ImGui::GetIO();
    if (input == "chinese")
        return io.Fonts->GetGlyphRangesChineseFull();
    else if (input == "simplified_chinese")
        return io.Fonts->GetGlyphRangesChineseSimplifiedCommon();
    else if (input == "cyrillic")
        return io.Fonts->GetGlyphRangesCyrillic();
    else if (input == "japanese")
        return io.Fonts->GetGlyphRangesJapanese();
    else if (input == "korean")
        return io.Fonts->GetGlyphRangesKorean();
    else if (input == "thai")
        return io.Fonts->GetGlyphRangesThai();
    else if (input == "vietnamese")
        return io.Fonts->GetGlyphRangesVietnamese();
    else
        return io.Fonts->GetGlyphRangesDefault();
}
