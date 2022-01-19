#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <utility/primitiveTypes.h>

namespace chira {
    class FileInputStream {
    public:
        FileInputStream(const std::string& filepath, bool binary = true) {
            this->stream.open(filepath, binary ? (std::ios::in | std::ios::binary) : std::ios::in);
        }
        ~FileInputStream() {
            this->stream.close();
        }
        FileInputStream(const FileInputStream& other) = delete;
        void operator=(const FileInputStream& other) = delete;
        FileInputStream(FileInputStream&& other) {
            this->stream = std::move(other.stream);
        }
        void operator=(FileInputStream&& other) {
            this->stream = std::move(other.stream);
        }
        std::vector<chira::byte> readBytes(unsigned int length) {
            std::vector<chira::byte> out;
            out.reserve(length);
            this->stream.read(reinterpret_cast<char*>(&out[0]), length);
            return out;
        }
        template<typename T>
        T read() {
            T wrongEndian = 0;
            this->stream.read(reinterpret_cast<char*>(&wrongEndian), sizeof(T));
            return swapEndian<T>(wrongEndian);
        }
        std::string readString() {
            std::string out;
            char temp;
            temp = this->read<char>();
            while (temp != '\0') {
                out += temp;
                temp = this->read<char>();
            }
            return out;
        }
        [[nodiscard]] const std::ifstream& getStream() const {
            return this->stream;
        }

        template<typename T>
        static inline T swapEndian(T t) {
            union {
                T t;
                chira::byte u8[sizeof(T)];
            } source{}, dest{};
            source.t = t;
            for (size_t k = 0; k < sizeof(T); k++)
                dest.u8[k] = source.u8[sizeof(T) - k - 1];
            return dest.t;
        }
    private:
        std::ifstream stream;
    };
}
