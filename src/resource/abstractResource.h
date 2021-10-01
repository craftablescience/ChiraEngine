#pragma once

#include <memory>
#include <string>

/// If adding a resource that depends on other resources,
/// release() them before release()ing the parent.
/// The destructor should discard the data.
class abstractResource {
    friend class resourceManager;
public:
    explicit abstractResource(std::string identifier_) : identifier(std::move(identifier_)) {}
    virtual void compile(unsigned char* buffer, std::size_t bufferLength) = 0;
    virtual void release() const;
    virtual ~abstractResource() = default;
protected:
    std::string identifier;
private:
    unsigned int refCount = 1;
    void incrementRefCount();
    void decrementRefCount();
    [[nodiscard]] unsigned int getRefCount() const;
};
