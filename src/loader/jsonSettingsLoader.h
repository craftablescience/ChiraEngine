#pragma once

#include <fstream>
#include "nlohmann/json.hpp"
#include "abstractFileSettingsLoader.h"

class jsonSettingsLoader : public abstractFileSettingsLoader {
public:
    explicit jsonSettingsLoader(const std::string& path);
    ~jsonSettingsLoader() override = default;

    void addCategory(const std::string& category) override;

    // Note: if the settings loader does not have the value, it should not raise an error, just don't modify the pointer
    void getValue(const std::string& category, const std::string& name, int* value) override;
    void getValue(const std::string& category, const std::string& name, double* value) override;
    void getValue(const std::string& category, const std::string& name, std::string* value) override;
    void getValue(const std::string& category, const std::string& name, bool* value) override;

    void setValue(const std::string& category, const std::string& name, int value, bool overwrite, bool save) override;
    void setValue(const std::string& category, const std::string& name, double value, bool overwrite, bool save) override;
    void setValue(const std::string& category, const std::string& name, const std::string& value, bool overwrite, bool save) override;
    void setValue(const std::string& category, const std::string& name, bool value, bool overwrite, bool save) override;

    bool hasCategory(const std::string& category) override;
    bool hasValue(const std::string& category, const std::string& name) override;
    void load() override;
    void save() override;
private:
    nlohmann::json settings;
};
