#pragma once

#include "uuid.h"
#include <memory>

namespace chira {
    class uuidGenerator {
    public:
        static uuids::uuid getNewUUID() {
            if (!uuidGenerator::isInitialized) {
                uuidGenerator::initializeGenerator();
            }
            return uuidGenerator::generator->operator()();
        }
        static std::string getNewUUIDString() {
            return uuids::to_string(uuidGenerator::getNewUUID());
        }
    private:
        static inline bool isInitialized = false;
        static inline std::unique_ptr<uuids::uuid_random_generator> generator = nullptr;

        static void initializeGenerator() {
            std::random_device rd;
            auto seedData = std::array<int, std::mt19937::state_size>{};
            std::generate(std::begin(seedData), std::end(seedData), std::ref(rd));
            std::seed_seq seq(std::begin(seedData), std::end(seedData));
            std::mt19937 mtEngine = std::mt19937{seq};
            uuidGenerator::generator = std::make_unique<uuids::uuid_random_generator>(mtEngine);
            uuidGenerator::isInitialized = true;
        }
    };
}
