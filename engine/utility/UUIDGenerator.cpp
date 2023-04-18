#include "UUIDGenerator.h"

using namespace chira;

UUIDGenerator::UUIDGeneratorInstance::UUIDGeneratorInstance() {
    std::generate(std::begin(this->seedData), std::end(this->seedData), std::ref(this->rd));
    std::seed_seq seq(std::begin(this->seedData), std::end(this->seedData));
    this->generator = std::make_unique<std::mt19937>(seq);
}

[[nodiscard]] std::mt19937* UUIDGenerator::UUIDGeneratorInstance::getGenerator() const {
    return this->generator.get();
}

uuids::uuid UUIDGenerator::getNewUUID() {
    if (!UUIDGenerator::generator) {
        UUIDGenerator::generator = std::make_unique<UUIDGeneratorInstance>();
    }
    return uuids::uuid_random_generator{*UUIDGenerator::generator->getGenerator()}();
}

std::string UUIDGenerator::getNewUUIDString() {
    return uuids::to_string(UUIDGenerator::getNewUUID());
}
