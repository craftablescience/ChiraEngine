#include "uuidGenerator.h"

using namespace chira;

std::unique_ptr<uuidGenerator::uuidGeneratorInstance> uuidGenerator::generator{};

uuidGenerator::uuidGeneratorInstance::uuidGeneratorInstance() {
    std::generate(std::begin(this->seedData), std::end(this->seedData), std::ref(this->rd));
    std::seed_seq seq(std::begin(this->seedData), std::end(this->seedData));
    this->generator = std::make_unique<std::mt19937>(seq);
}

[[nodiscard]] std::mt19937* uuidGenerator::uuidGeneratorInstance::getGenerator() const {
    return this->generator.get();
}

uuids::uuid uuidGenerator::getNewUUID() {
    if (!uuidGenerator::generator) {
        uuidGenerator::generator = std::make_unique<uuidGeneratorInstance>();
    }
    return uuids::uuid_random_generator{*uuidGenerator::generator->getGenerator()}();
}

std::string uuidGenerator::getNewUUIDString() {
    return uuids::to_string(uuidGenerator::getNewUUID());
}
