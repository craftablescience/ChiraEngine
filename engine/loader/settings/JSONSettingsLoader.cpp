#include "JSONSettingsLoader.h"

#include <fstream>
#include <iomanip>

using namespace chira;

JSONSettingsLoader::JSONSettingsLoader(std::string_view path)
    : ISettingsLoader(std::move(path))
    , settings() {}

void JSONSettingsLoader::addCategory(const std::string& category) {
    if (!this->settings.contains(category)) {
        this->settings[category] = nlohmann::json::object();
    }
}

void JSONSettingsLoader::getValue(const std::string& category, const std::string& name, int* value) const {
    if (this->hasValue(category, name)) {
        *value = this->settings[category][name];
    }
}

void JSONSettingsLoader::getValue(const std::string& category, const std::string& name, double* value) const {
    if (this->hasValue(category, name)) {
        *value = this->settings[category][name];
    }
}

void JSONSettingsLoader::getValue(const std::string& category, const std::string& name, std::string* value) const {
    if (this->hasValue(category, name)) {
        *value = this->settings[category][name];
    }
}

void JSONSettingsLoader::getValue(const std::string& category, const std::string& name, bool* value) const {
    if (this->hasValue(category, name)) {
        *value = this->settings[category][name];
    }
}

void JSONSettingsLoader::setValue(const std::string& category, const std::string& name, int value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(category, name)) {
        return;
    }
    if (!this->hasCategory(category)) {
        this->addCategory(category);
    }
    this->settings[category][name] = value;
    if (save) {
        this->save();
    }
}

void JSONSettingsLoader::setValue(const std::string& category, const std::string& name, double value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(category, name)) {
        return;
    }
    if (!this->hasCategory(category)) {
        this->addCategory(category);
    }
    this->settings[category][name] = value;
    if (save) {
        this->save();
    }
}

void JSONSettingsLoader::setValue(const std::string& category, const std::string& name, const std::string& value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(category, name)) {
        return;
    }
    if (!this->hasCategory(category)) {
        this->addCategory(category);
    }
    this->settings[category][name] = value;
    if (save) {
        this->save();
    }
}

void JSONSettingsLoader::setValue(const std::string& category, const std::string& name, bool value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(category, name)) {
        return;
    }
    if (!this->hasCategory(category)) {
        this->addCategory(category);
    }
    this->settings[category][name] = value;
    if (save) {
        this->save();
    }
}

bool JSONSettingsLoader::hasCategory(const std::string& category) const {
    return this->settings.contains(category);
}

bool JSONSettingsLoader::hasValue(const std::string& category, const std::string& name) const {
    return this->settings.contains(category) && this->settings[category].contains(name);
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
        if (this->hasCategory(element.key())) {
            for (auto value = element.value().begin(); value != element.value().end(); ++value) {
                this->settings[element.key()][value.key()] = value.value();
            }
        } else {
            this->settings[element.key()] = element.value();
        }
    }
}

void JSONSettingsLoader::save() {
    std::ofstream output{this->getFilePath().data()};
    output << std::setw(4) << this->settings << std::endl;
}
