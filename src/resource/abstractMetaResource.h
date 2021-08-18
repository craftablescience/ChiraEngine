#pragma once

#include "abstractResource.h"
#include <nlohmann/json.hpp>

/*
 * Suggested format:
 *
 * {
 *     "provider": "(set by the compile function, not part of the file until compiled)",
 *     "dependencies": {
 *         "dep1": "path",
 *         "dep2": "path"
 *     },
 *     "properties": {
 *         "prop1": "value1",
 *         "prop2": "value2"
 *     }
 * }
 *
 * Dependencies and properties are handled by derivative classes,
 * this is only the recommendation on how to store meta-resource files.
 */
class abstractMetaResource : public abstractResource {
public:
    explicit abstractMetaResource(const std::string& provider_, const std::string& name_) : abstractResource(provider_, name_) {}
    void compile(std::unique_ptr<unsigned char> buffer, unsigned int bufferLength) final {
        // todo: convert buffer to json, pass it to compile(props)
    }
    virtual void compile(const nlohmann::json& properties) = 0;
};
