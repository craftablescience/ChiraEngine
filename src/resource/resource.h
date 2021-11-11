#pragma once

#include <memory>
#include <string>

namespace chira {
    /// If adding a resource that depends on other resources,
    /// release() them before release()ing the parent.
    /// The destructor should discard the data.
    class resource {
        friend class resourceManager;
    public:
        explicit resource(std::string identifier_) : identifier(std::move(identifier_)) {}
        virtual void compile(const unsigned char buffer[], std::size_t bufferLength) {}
        [[nodiscard]] const std::string& getIdentifier() const noexcept {
            return this->identifier;
        }
        virtual ~resource();
    protected:
        std::string identifier;
    };
}
