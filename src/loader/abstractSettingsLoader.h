#pragma once

#include <string>

class abstractSettingsLoader {
public:
    virtual ~abstractSettingsLoader() = default;

    virtual void addCategory(const std::string& category) = 0;

    // Note: if the settings loader does not have the value, it should not raise an error, just don't modify the pointer
    virtual void getValue(const std::string& category, const std::string& name, int* value) = 0;
    virtual void getValue(const std::string& category, const std::string& name, double* value) = 0;
    virtual void getValue(const std::string& category, const std::string& name, std::string* value) = 0;
    virtual void getValue(const std::string& category, const std::string& name, bool* value) = 0;

    virtual void setValue(const std::string& category, const std::string& name, int value, bool overwrite, bool save) = 0;
    virtual void setValue(const std::string& category, const std::string& name, double value, bool overwrite, bool save) = 0;
    virtual void setValue(const std::string& category, const std::string& name, const std::string& value, bool overwrite, bool save) = 0;
    virtual void setValue(const std::string& category, const std::string& name, bool value, bool overwrite, bool save) = 0;

    virtual bool hasCategory(const std::string& category) = 0;
    virtual bool hasValue(const std::string& category, const std::string& name) = 0;
    virtual void load() = 0;
    virtual void save() = 0;
};
