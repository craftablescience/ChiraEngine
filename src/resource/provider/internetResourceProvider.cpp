#include "internetResourceProvider.h"

#include <fstream>
#include <algorithm>
#include "curlpp/cURLpp.hpp"
#include "curlpp/Easy.hpp"
#include "curlpp/Options.hpp"
#include "../../utility/logger.h"
#include <fmt/core.h>
#include "../../utility/curlMemoryWriter.h"

using namespace chira;

void internetResourceProvider::compileResource(const std::string& name, resource* resource) {
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
}
