#pragma once

#include "uuid.h"
#include <memory>

namespace chira {
    class uuidGenerator {
    private:
        class uuidGeneratorInstance {
        public:
            uuidGeneratorInstance();
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
        static inline std::unique_ptr<uuidGeneratorInstance> generator;
    };
}
