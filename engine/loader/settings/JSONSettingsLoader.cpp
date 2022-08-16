#include "JSONSettingsLoader.h"

#include <fstream>
#include <iomanip>

using namespace chira;

JSONSettingsLoader::JSONSettingsLoader(std::string_view path, std::string_view filename)
    : ISettingsLoader(path, filename)
    , settings() {
    this->load();
}

void JSONSettingsLoader::getValue(const std::string& name, int* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONSettingsLoader::getValue(const std::string& name, double* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONSettingsLoader::getValue(const std::string& name, std::string* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONSettingsLoader::getValue(const std::string& name, bool* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONSettingsLoader::setValue(const std::string& name, int value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

void JSONSettingsLoader::setValue(const std::string& name, double value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

void JSONSettingsLoader::setValue(const std::string& name, const std::string& value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

void JSONSettingsLoader::setValue(const std::string& name, bool value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

bool JSONSettingsLoader::hasValue(const std::string& name) const {
    return this->settings.contains(name);
}

void JSONSettingsLoader::load() {
    std::ifstream fileCheck{this->getFilePath().data()};
    if (!fileCheck.good()) {
        // Make new file if it doesn't exist
        std::fstream fs{this->getFilePath().data(), std::fstream::out};
        fs << "{}\n";
        fs.close();
    }
    std::ifstream inputFile{this->getFilePath().data()};
    nlohmann::json input;
    inputFile >> input;
    inputFile.close();
    for (auto element = input.begin(); element != input.end(); ++element) {
        this->settings[element.key()] = element.value();
    }
}

void JSONSettingsLoader::save() {
    std::ofstream output{this->getFilePath().data()};
    output << std::setw(4) << this->settings << std::endl;
}
