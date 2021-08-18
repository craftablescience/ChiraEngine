#pragma once

/*
 * The destructor should discard the data
 */
class abstractResource {
public:
    explicit abstractResource(std::string provider_, std::string name_) : provider(std::move(provider_)), name(std::move(name_)) {}
    virtual void compile(std::unique_ptr<unsigned char> buffer, unsigned int bufferLength) = 0;
    void removeIfUnused() const;
    virtual ~abstractResource();
protected:
    std::string provider;
    std::string name;
};
