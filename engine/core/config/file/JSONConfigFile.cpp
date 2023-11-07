#include "JSONConfigFile.h"

#include <fstream>
#include <iomanip>

#include "../Config.h"

using namespace chira;

JSONConfigFile::JSONConfigFile(std::string_view filename)
    : IConfigFile(filename, Config::getConfigDirectory(), false) {
    this->load();
}

JSONConfigFile::JSONConfigFile(std::string_view filename, std::string_view path, bool relative /*= false*/)
    : IConfigFile(filename, path, relative) {
    this->load();
}

void JSONConfigFile::getValue(const std::string& name, int* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONConfigFile::getValue(const std::string& name, double* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONConfigFile::getValue(const std::string& name, std::string* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONConfigFile::getValue(const std::string& name, bool* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONConfigFile::setValue(const std::string& name, int value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

void JSONConfigFile::setValue(const std::string& name, double value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

void JSONConfigFile::setValue(const std::string& name, const std::string& value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

void JSONConfigFile::setValue(const std::string& name, bool value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

bool JSONConfigFile::hasValue(const std::string& name) const {
    return this->settings.contains(name);
}

void JSONConfigFile::load() {
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

void JSONConfigFile::save() {
    std::ofstream output{this->getFilePath().data()};
    output << std::setw(4) << this->settings << std::endl;
}
