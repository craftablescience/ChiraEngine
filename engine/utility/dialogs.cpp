#include "dialogs.h"

#include <filesystem>
#include <tinyfiledialogs.h>
#include <i18n/translationManager.h>
#include <resource/provider/filesystemResourceProvider.h>

std::string chira::openFile(const std::string& pattern, const std::string& description) {
    const char* filter[1] = {pattern.c_str()};
    std::string path;
    auto result = tinyfd_openFileDialog(
            TR("ui.window.select_file").c_str(),
            std::filesystem::current_path().string().c_str(), 1,
            filter, description.empty() ? nullptr : description.c_str(), 0);
    if (result) {
        path = result;
        return path;
    }
    return "";
}

std::string chira::openResource(const std::string& pattern, const std::string& description) {
    const char* filter[1] = {pattern.c_str()};
    auto result = tinyfd_openFileDialog(
            TR("ui.window.select_file").c_str(),
            std::filesystem::current_path().string().c_str(), 1,
            filter, description.empty() ? nullptr : description.c_str(), 0);
    if (result)
        return filesystemResourceProvider::getResourcePath(std::string{result});
    return "";
}
