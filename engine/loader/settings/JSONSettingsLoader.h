#pragma once

#include <nlohmann/json.hpp>

#include "ISettingsLoader.h"

namespace chira {

class JSONSettingsLoader : public ISettingsLoader {
public:
    explicit JSONSettingsLoader(std::string_view filename);
    JSONSettingsLoader(std::string_view filename, std::string_view path, bool relative);

    void getValue(const std::string& name, int* value) const override;
    void getValue(const std::string& name, double* value) const override;
    void getValue(const std::string& name, std::string* value) const override;
    void getValue(const std::string& name, bool* value) const override;

    void setValue(const std::string& name, int value, bool overwrite, bool save) override;
    void setValue(const std::string& name, double value, bool overwrite, bool save) override;
    void setValue(const std::string& name, const std::string& value, bool overwrite, bool save) override;
    void setValue(const std::string& name, bool value, bool overwrite, bool save) override;

    [[nodiscard]] bool hasValue(const std::string& name) const override;

    void load() final;
    void save() final;
private:
    nlohmann::json settings{};
};

} // namespace chira
