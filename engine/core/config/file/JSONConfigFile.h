#pragma once

#include <nlohmann/json.hpp>

#include "IConfigFile.h"

namespace chira {

class JSONConfigFile : public IConfigFile {
public:
	JSONConfigFile();

    void getValue(std::string_view name, int* value) const override;
    void getValue(std::string_view name, double* value) const override;
    void getValue(std::string_view name, std::string* value) const override;
    void getValue(std::string_view name, bool* value) const override;

    void setValue(std::string_view name, int value, bool overwrite, bool save) override;
    void setValue(std::string_view name, double value, bool overwrite, bool save) override;
    void setValue(std::string_view name, std::string_view value, bool overwrite, bool save) override;
    void setValue(std::string_view name, bool value, bool overwrite, bool save) override;

    bool hasValue(std::string_view name) const override;

    void load() override;
    void save() override;

private:
    nlohmann::json settings{};
};

} // namespace chira
