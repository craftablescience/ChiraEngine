#pragma once

#include <memory>
#include <uuid.h>

namespace chira {
    class UUIDGenerator {
    private:
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
        static std::unique_ptr<UUIDGeneratorInstance> generator;
    };
}
