#pragma once

#include <memory>
#include <string>

/*
 * If adding a resource that depends on other resources,
 * release() them before release()ing it
 * The destructor should discard the data
 */
class abstractResource {
    friend class resourceManager;
public:
    abstractResource(std::string provider_, std::string name_) : provider(std::move(provider_)), name(std::move(name_)) {}
    virtual void compile(unsigned char* buffer, std::size_t bufferLength) = 0;
    virtual void release() const;
    virtual ~abstractResource() = default;
protected:
    std::string provider;
    std::string name;
private:
    unsigned int refCount = 1;
    void incrementRefCount();
    void decrementRefCount();
    [[nodiscard]] unsigned int getRefCount() const;
};
