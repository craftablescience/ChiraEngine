#pragma once

#include "uuid.h"

namespace chira {
    class uuidGenerator {
    public:
        static uuids::uuid getNewUUID() {
            if (!uuidGenerator::isInitialized) {
                uuidGenerator::initializeGenerator();
            }
            return uuidGenerator::generator();
        }
        static std::string getNewUUIDString() {
            return uuids::to_string(uuidGenerator::getNewUUID());
        }
    private:
        static inline bool isInitialized = false;
        static std::mt19937 engine;
        static uuids::uuid_random_generator generator;

        static void initializeGenerator() {
            std::random_device rd;
            auto seedData = std::array<int, std::mt19937::state_size>{};
            std::generate(std::begin(seedData), std::end(seedData), std::ref(rd));
            std::seed_seq seq(std::begin(seedData), std::end(seedData));
            uuidGenerator::engine = std::mt19937{seq};
            uuidGenerator::generator = uuids::uuid_random_generator{uuidGenerator::engine};
            uuidGenerator::isInitialized = true;
        }
    };
}
