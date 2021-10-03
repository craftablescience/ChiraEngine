#pragma once

#include <string>
#include "abstractResourceProvider.h"

namespace chira {
    /// Note: this provider must only be registered once under one name / protocol!
    /// It always returns true when asked if it holds a resource, this may not actually be true however.
    /// Additionally, the "name" passed to it is the protocol it uses (e.g. http, https, ftp, etc.)
    class internetResourceProvider : public abstractResourceProvider {
    public:
        explicit internetResourceProvider(const std::string& name_, unsigned short port_) : abstractResourceProvider(name_), port(port_) {}
        bool hasResource(const std::string& name) override {
            return true;
        }
        void compileResource(const std::string& name, abstractResource* resource) override;
        [[nodiscard]] const std::string& getProtocol() const {
            return this->providerName;
        }
        [[nodiscard]] unsigned short getPort() const {
            return this->port;
        }
    private:
        unsigned short port;
    };
}
