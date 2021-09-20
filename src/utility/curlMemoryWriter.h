#pragma once

#include <utility>

class curlMemoryWriter {
public:
    curlMemoryWriter();
    ~curlMemoryWriter();
    static void* w_realloc(void* ptr, std::size_t size);
    std::size_t writeMemoryCallback(const char* ptr, std::size_t size, std::size_t nmemb);

    unsigned char* m_pBuffer;
    std::size_t m_Size;
};
