#pragma once

#include "abstractResourceProvider.h"

namespace chira {
    /// Note: this provider must only be registered once under one name / protocol!
    /// It always returns true when asked if it holds a resource, this may not actually be true however.
    /// Additionally, the "name" passed to it is the protocol it uses (e.g. http, https, ftp, etc.)
    class InternetResourceProvider : public AbstractResourceProvider {
    public:
        explicit InternetResourceProvider(const std::string& name_, unsigned short port_)
            : AbstractResourceProvider(name_)
            , port(port_) {}
        /// Redefines this function to always return true.
        /// This is better than sending a request to the URI and seeing if it's valid, because that will take a lot of time.
        /// It lets us request the resource immediately, but you must know beforehand that it's a valid URI.
        [[nodiscard]] bool hasResource(const std::string& name) const override {
            return true;
        }
        void compileResource(const std::string& name, Resource* resource) const override;
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
