#include "FontResource.h"

#include "provider/FilesystemResourceProvider.h"

using namespace chira;

void FontResource::compile(const nlohmann::json& properties) {
    Serialize::fromJSON(this, properties);
    ImGuiIO& io = ImGui::GetIO();
    this->range = FontResource::getRangeFromString(this->rangeStr);
    std::string path = FilesystemResourceProvider::getResourceAbsolutePath(this->fontPath);
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

const ImWchar* FontResource::getRangeFromString(std::string_view input) {
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
