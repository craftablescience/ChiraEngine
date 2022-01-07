#pragma once

#include <string>

namespace chira {
    std::string stripLeft(const std::string& s);
    std::string stripRight(const std::string& s);
    std::string strip(const std::string& s);
    std::string stripLeft(const std::string& s, const std::string& c);
    std::string stripRight(const std::string& s, const std::string& c);
    std::string strip(const std::string& s, const std::string& c);
    std::string stripLeft(const std::string& s, char c);
    std::string stripRight(const std::string& s, char c);
    std::string strip(const std::string& s, char c);
    void stripLeftModify(std::string& s);
    void stripRightModify(std::string& s);
    void stripModify(std::string& s);
    void stripLeftModify(std::string& s, const std::string& c);
    void stripRightModify(std::string& s, const std::string& c);
    void stripModify(std::string& s, const std::string& c);
    void stripLeftModify(std::string& s, char c);
    void stripRightModify(std::string& s, char c);
    void stripModify(std::string& s, char c);
}
