#pragma once

#include <string>

namespace chira {
    std::string stripLeft(const std::string& s);
    std::string stripRight(const std::string& s);
    std::string strip(const std::string& s);
    std::string stripLeft(const std::string& s, unsigned char c);
    std::string stripRight(const std::string& s, unsigned char c);
    std::string strip(const std::string& s, unsigned char c);
    void stripLeftModify(std::string& s);
    void stripRightModify(std::string& s);
    void stripModify(std::string& s);
    void stripLeftModify(std::string& s, unsigned char c);
    void stripRightModify(std::string& s, unsigned char c);
    void stripModify(std::string& s, unsigned char c);
}
