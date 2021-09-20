#include "internetResourceProvider.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"
#include "../utility/logger.h"
#include "fmt/core.h"

void internetResourceProvider::compileResource(const std::string& name, abstractResource* resource) {
    curlpp::Easy request;
    std::fstream fs;
    try {
        request.setOpt(curlpp::options::Url(this->providerName + name));
        request.setOpt(curlpp::options::Port(this->port));
        fs << request;
    } catch (curlpp::RuntimeError& e) {
        chira::logger::log(ERR, fmt::format("Internet Resource Provider ({}:{})", this->providerName, this->port), e.what());
    } catch (curlpp::LogicError& e) {
        chira::logger::log(ERR, fmt::format("Internet Resource Provider ({}:{})", this->providerName, this->port), e.what());
    }
    std::vector<unsigned char> bytes;
    std::copy(std::istream_iterator<unsigned char>(fs), std::istream_iterator<unsigned char>(), std::back_inserter(bytes));
    resource->compile(&bytes[0], bytes.size());
}
