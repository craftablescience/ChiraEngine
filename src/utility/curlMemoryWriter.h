#pragma once

#include <utility>

namespace chira {
    struct curlMemoryWriter {
        constexpr static int MAX_BUFFER_LENGTH = 20000;
        curlMemoryWriter();
        ~curlMemoryWriter();
        static void* w_realloc(void* ptr, std::size_t size);
        std::size_t writeMemoryCallback(const char* ptr, std::size_t size, std::size_t nmemb);

        // The following variables are publicly accessed in internetResourceProvider

        unsigned char* m_pBuffer;
        std::size_t m_Size;
    };
}
