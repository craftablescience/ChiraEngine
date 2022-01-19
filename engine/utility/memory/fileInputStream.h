#pragma once

#include <cstring>
#include <cstdio>
#include <string>
#include <vector>

#include <utility/primitiveTypes.h>

namespace chira {
    class FileInputStream {
    public:
        explicit FileInputStream(const std::string& filepath, bool binary = true);
        ~FileInputStream();
        FileInputStream(const FileInputStream& other) = delete;
        FileInputStream& operator=(const FileInputStream& other) = delete;
        FileInputStream(FileInputStream&& other) noexcept;
        FileInputStream& operator=(FileInputStream&& other) noexcept;
        explicit operator bool() const;
        bool operator!() const;
        void seek(long pos) const;
        void seek(long offset, int offsetFrom) const;
        [[nodiscard]] long tell() const;
        [[nodiscard]] std::vector<chira::byte> readBytes(unsigned int length) const;
        template<typename T>
        T read(bool swapEndian_ = false) const {
            T wrongEndian = 0;
            auto bytes = this->readBytes(sizeof(T));
            std::memcpy(&wrongEndian, &bytes[0], sizeof(T));
            return swapEndian_ ? swapEndian<T>(wrongEndian) : wrongEndian;
        }
        [[nodiscard]] std::string readString() const;
        [[nodiscard]] byte peek(long offset = 0) const;
    protected:
        FILE* stream;
    };
}
