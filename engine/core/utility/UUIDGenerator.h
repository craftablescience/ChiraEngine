#pragma once

#include <memory>

#include <nlohmann/json.hpp>
#include <uuid.h>

namespace chira {

class UUIDGenerator {
    class UUIDGeneratorInstance {
    public:
        UUIDGeneratorInstance();
        [[nodiscard]] std::mt19937* getGenerator() const;
    private:
        std::random_device rd{};
        std::array<int, std::mt19937::state_size> seedData{};
        std::unique_ptr<std::mt19937> generator;
    };

public:
    static uuids::uuid getNewUUID();
    static std::string getNewUUIDString();
private:
    static inline std::unique_ptr<UUIDGeneratorInstance> generator{};
};

} // namespace chira

namespace uuids {

inline void to_json(nlohmann::json& j, const uuid& u) {
    j = {to_string(u)};
}

inline void from_json(const nlohmann::json& j, uuid& u) {
    u = *uuid::from_string(j.get<std::string>());
}

} // namespace uuids
