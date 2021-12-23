#include "jsonSettingsLoader.h"

#include <fstream>
#include <iomanip>

using namespace chira;

jsonSettingsLoader::jsonSettingsLoader(const std::string& path) : abstractFileSettingsLoader(path), settings() {}

void jsonSettingsLoader::addCategory(const std::string& category) {
    if (!this->settings.contains(category)) {
        this->settings[category] = nlohmann::json::object();
    }
}

void jsonSettingsLoader::getValue(const std::string& category, const std::string& name, int* value) {
    if (this->hasValue(category, name)) {
        *value = this->settings[category][name];
    }
}

void jsonSettingsLoader::getValue(const std::string& category, const std::string& name, double* value) {
    if (this->hasValue(category, name)) {
        *value = this->settings[category][name];
    }
}

void jsonSettingsLoader::getValue(const std::string& category, const std::string& name, std::string* value) {
    if (this->hasValue(category, name)) {
        *value = this->settings[category][name];
    }
}

void jsonSettingsLoader::getValue(const std::string& category, const std::string& name, bool* value) {
    if (this->hasValue(category, name)) {
        *value = this->settings[category][name];
    }
}

void jsonSettingsLoader::setValue(const std::string& category, const std::string& name, int value, bool overwrite, bool save) {
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

void jsonSettingsLoader::setValue(const std::string& category, const std::string& name, double value, bool overwrite, bool save) {
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

void jsonSettingsLoader::setValue(const std::string& category, const std::string& name, const std::string& value, bool overwrite, bool save) {
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

void jsonSettingsLoader::setValue(const std::string& category, const std::string& name, bool value, bool overwrite, bool save) {
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

bool jsonSettingsLoader::hasCategory(const std::string& category) {
    return this->settings.contains(category);
}

bool jsonSettingsLoader::hasValue(const std::string& category, const std::string& name) {
    return this->settings.contains(category) && this->settings[category].contains(name);
}

void jsonSettingsLoader::load() {
    std::ifstream fileCheck(this->getFilePath());
    if (!fileCheck.good()) {
        // Make new file if it doesn't exist
        std::fstream fs(this->getFilePath(), std::fstream::out);
        fs << "{}\n";
        fs.close();
    }
    std::ifstream inputFile(this->getFilePath());
    nlohmann::json input;
    inputFile >> input;
    inputFile.close();
    for (nlohmann::json::iterator element = input.begin(); element != input.end(); ++element) {
        if (this->hasCategory(element.key())) {
            for (nlohmann::json::iterator value = element.value().begin(); value != element.value().end(); ++value) {
                this->settings[element.key()][value.key()] = value.value();
            }
        } else {
            this->settings[element.key()] = element.value();
        }
    }
}

void jsonSettingsLoader::save() {
    std::ofstream output(this->getFilePath());
    output << std::setw(4) << this->settings << std::endl;
}
