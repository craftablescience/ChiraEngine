#include "Font.h"

using namespace chira;

void Font::compile(const byte buffer[], std::size_t bufferLength) {
    Serial::loadFromBuffer(this, buffer, bufferLength);

    ImGuiIO& io = ImGui::GetIO();
    this->range = Font::getRangeFromString(this->rangeStr);
    std::string path = "";//FilesystemResourceProvider::getResourceAbsolutePath(this->fontPath);
    this->font = io.Fonts->AddFontFromFileTTF(path.c_str(), this->size, nullptr, this->range);
}

ImFont* Font::getFont() const {
    return this->font;
}

const std::string& Font::getName() const {
    return this->name;
}

float Font::getSize() const {
    return this->size;
}

const ImWchar* Font::getRangeFromString(std::string_view input) {
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
    else if (input == "english")
        return io.Fonts->GetGlyphRangesDefault();
    return io.Fonts->GetGlyphRangesDefault();
}
