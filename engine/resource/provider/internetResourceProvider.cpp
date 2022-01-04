#include "internetResourceProvider.h"

#include <algorithm>
#include <fmt/core.h>
#include <utility/logger.h>
#include <utility/curlMemoryWriter.h>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <resource/resource.h>

using namespace chira;

void InternetResourceProvider::compileResource(const std::string& name, Resource* resource) {
    curlpp::Easy request;
    CurlMemoryWriter mWriterChunk;
    try {
        request.setOpt(curlpp::options::WriteFunction(std::bind(&CurlMemoryWriter::writeMemoryCallback, &mWriterChunk, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
        request.setOpt(curlpp::options::Url(this->providerName + "://" + name));
        request.setOpt(curlpp::options::Port(this->port));
        request.perform();
        auto buffer = new unsigned char[mWriterChunk.m_Size + 1];
        memcpy(buffer, mWriterChunk.m_pBuffer, mWriterChunk.m_Size);
        buffer[mWriterChunk.m_Size] = '\0';
        resource->compile(buffer, mWriterChunk.m_Size + 1);
        delete[] buffer;
    }
    // Go to hell macros
#pragma push_macro("ERROR")
#undef ERROR
    catch (curlpp::RuntimeError& e) {
        Logger::log(LogType::ERROR, fmt::format("Internet Resource Provider ({}:{})", this->providerName, this->port), e.what());
    } catch (curlpp::LogicError& e) {
        Logger::log(LogType::ERROR, fmt::format("Internet Resource Provider ({}:{})", this->providerName, this->port), e.what());
    }
#pragma pop_macro("ERROR")
}
