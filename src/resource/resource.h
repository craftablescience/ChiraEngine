#pragma once

#include <memory>
#include <string>
#include "../../utility/sharedPointer.h"

namespace chira {
    /// If adding a resource that depends on other resources,
    /// release() them before release()ing the parent.
    /// The destructor should discard the data.
    class resource {
        friend class resourceManager;
    public:
        explicit resource(std::string identifier_) : identifier(std::move(identifier_)) {}
        virtual ~resource();
        virtual void compile(const unsigned char buffer[], std::size_t bufferLength) {}
        [[nodiscard]] const std::string& getIdentifier() const noexcept {
            return this->identifier;
        }
    protected:
        std::string identifier;
    };
}
