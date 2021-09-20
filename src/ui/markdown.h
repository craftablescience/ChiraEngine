#pragma once

#include "imgui.h"
#include "imgui_markdown.h"
#if WIN32
#include <windows.h>
#include <shellapi.h>
#endif
#include <string>

class markdown {
public:
    /*
    void LoadFonts(float fontSize_ = 12.0f) {
        // todo: get H1, H2, and H3 fonts
        ImGuiIO &io = ImGui::GetIO();
        io.Fonts->Clear();
        // Base font
        io.Fonts->AddFontFromFileTTF("myfont.ttf", fontSize_);
        // Bold headings H2 and H3
        H2 = io.Fonts->AddFontFromFileTTF("myfont-bold.ttf", fontSize_);
        H3 = mdConfig.headingFormats[1].font;
        // bold heading H1
        float fontSizeH1 = fontSize_ * 1.1f;
        H1 = io.Fonts->AddFontFromFileTTF("myfont-bold.ttf", fontSizeH1);
    }
    */
    static void create(const std::string& markdown_) {
        markdown::mdConfig.linkCallback = markdown::linkCallback;
        //markdown::mdConfig.headingFormats[0] = {H1, true};
        //markdown::mdConfig.headingFormats[1] = {H2, true};
        //markdown::mdConfig.headingFormats[2] = {H3, false};
        markdown::mdConfig.userData = nullptr;
        ImGui::Markdown(markdown_.c_str(), markdown_.length(), mdConfig);
    }
private:
    static inline ImGui::MarkdownConfig mdConfig{};

    static void linkCallback(ImGui::MarkdownLinkCallbackData data_) {
        std::string url(data_.link, data_.linkLength);
        if (!data_.isImage) {
#if WIN32
            ShellExecuteA(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#else
            system(("open " + url).c_str());
#endif
        }
    }
};
