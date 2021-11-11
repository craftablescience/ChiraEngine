#include "internetResourceProvider.h"

#include <fstream>
#include <algorithm>
#include <fmt/core.h>
#include "../../utility/logger.h"
#ifdef CHIRA_BUILD_CURLPP_ENABLED
#include "../../utility/curlMemoryWriter.h"
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"
#endif

using namespace chira;

void internetResourceProvider::compileResource(const std::string& name, resource* resource) {
#ifdef CHIRA_BUILD_CURLPP_ENABLED
    curlpp::Easy request;
    curlMemoryWriter mWriterChunk;
    try {
        request.setOpt(curlpp::options::WriteFunction(std::bind(&curlMemoryWriter::writeMemoryCallback, &mWriterChunk, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
        request.setOpt(curlpp::options::Url(this->providerName + "://" + name));
        request.setOpt(curlpp::options::Port(this->port));
        request.perform();
        resource->compile(mWriterChunk.m_pBuffer, mWriterChunk.m_Size);
    } catch (curlpp::RuntimeError& e) {
        logger::log(ERR, fmt::format("Internet Resource Provider ({}:{})", this->providerName, this->port), e.what());
    } catch (curlpp::LogicError& e) {
        logger::log(ERR, fmt::format("Internet Resource Provider ({}:{})", this->providerName, this->port), e.what());
    }
#else
    //todo(localize)
    logger::log(ERR, "Internet Resource Provider", "This feature is disabled. The resource has not been compiled.");
#endif
}
