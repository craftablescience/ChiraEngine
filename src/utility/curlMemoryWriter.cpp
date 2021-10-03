#include "curlMemoryWriter.h"

#include <cstdlib>
#include <cstring>

#define MAX_BUFFER_LENGTH 20000

using namespace chira;

curlMemoryWriter::curlMemoryWriter() {
    this->m_pBuffer = nullptr;
    this->m_pBuffer = (unsigned char*) std::malloc(MAX_BUFFER_LENGTH * sizeof(unsigned char));
    this->m_Size = 0;
}

curlMemoryWriter::~curlMemoryWriter() {
    if (this->m_pBuffer) {
        std::free(this->m_pBuffer);
    }
}

void* curlMemoryWriter::w_realloc(void* ptr, std::size_t size) {
    if (ptr) {
        return std::realloc(ptr, size);
    } else {
        return std::malloc(size);
    }
}

std::size_t curlMemoryWriter::writeMemoryCallback(const char* ptr, std::size_t size, std::size_t nmemb) {
    // Calculate the real size of the incoming buffer
    std::size_t realsize = size * nmemb;

    // (Re)Allocate memory for the buffer
    m_pBuffer = (unsigned char*) this->w_realloc(m_pBuffer, m_Size + realsize);

    // Test if Buffer is initialized correctly & copy memory
    if (m_pBuffer == nullptr) {
        realsize = 0;
    }
    std::memcpy(&(m_pBuffer[m_Size]), ptr, realsize);
    m_Size += realsize;
    return realsize;
}
