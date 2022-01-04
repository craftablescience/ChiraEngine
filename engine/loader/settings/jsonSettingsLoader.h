#pragma once

#include <nlohmann/json.hpp>
#include "abstractFileSettingsLoader.h"

namespace chira {
    class JSONSettingsLoader : public AbstractFileSettingsLoader {
    public:
        explicit JSONSettingsLoader(const std::string& path);
        ~JSONSettingsLoader() override = default;

        void addCategory(const std::string& category) override;

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
}
