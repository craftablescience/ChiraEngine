#pragma once

#include <string>
#include <string_view>

namespace chira {

class IConfigFile {
public:
	IConfigFile() = default;

    virtual ~IConfigFile() = default;

	void open(std::string_view path) {
		this->filepath = path;
		this->load();
	}

	[[nodiscard]] bool isOpen() const {
		return !this->filepath.empty();
	}

    virtual void getValue(std::string_view name, int* value) const = 0;
    virtual void getValue(std::string_view name, double* value) const = 0;
    virtual void getValue(std::string_view name, std::string* value) const = 0;
    virtual void getValue(std::string_view name, bool* value) const = 0;

    virtual void setValue(std::string_view name, int value, bool overwrite, bool save) = 0;
    virtual void setValue(std::string_view name, double value, bool overwrite, bool save) = 0;
    virtual void setValue(std::string_view name, std::string_view value, bool overwrite, bool save) = 0;
    virtual void setValue(std::string_view name, bool value, bool overwrite, bool save) = 0;

    [[nodiscard]] virtual bool hasValue(std::string_view name) const = 0;

    virtual void load() = 0;
    virtual void save() = 0;

    [[nodiscard]] std::string_view getFilePath() const {
        return this->filepath;
    }

private:
    std::string filepath;
};

} // namespace chira
