#include "JSONConfigFile.h"

#include <fstream>
#include <iomanip>

#include "../Config.h"

using namespace chira;

JSONConfigFile::JSONConfigFile()
		: IConfigFile() {}

void JSONConfigFile::getValue(std::string_view name, int* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONConfigFile::getValue(std::string_view name, double* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONConfigFile::getValue(std::string_view name, std::string* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONConfigFile::getValue(std::string_view name, bool* value) const {
    if (this->hasValue(name)) {
        *value = this->settings[name];
    }
}

void JSONConfigFile::setValue(std::string_view name, bool value, bool overwrite, bool save) {
	if (!overwrite && this->hasValue(name)) {
		return;
	}
	this->settings[name] = value;
	if (save) {
		this->save();
	}
}

void JSONConfigFile::setValue(std::string_view name, int value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

void JSONConfigFile::setValue(std::string_view name, double value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = value;
    if (save) {
        this->save();
    }
}

void JSONConfigFile::setValue(std::string_view name, std::string_view value, bool overwrite, bool save) {
    if (!overwrite && this->hasValue(name)) {
        return;
    }
    this->settings[name] = std::string{value};
    if (save) {
        this->save();
    }
}

bool JSONConfigFile::hasValue(std::string_view name) const {
    return this->settings.contains(name);
}

void JSONConfigFile::load() {
	// todo(fs)

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
	// todo(fs)

    std::ofstream output{this->getFilePath().data()};
    output << std::setw(4) << this->settings << std::endl;
}
