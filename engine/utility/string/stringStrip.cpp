#include "stringStrip.h"

#include <regex>

// Modified from https://linuxhint.com/trim-string-cpp

std::string chira::stripLeft(const std::string& s) {
    return std::regex_replace(s, std::regex("^[ |\t|\n|\r|\v|\f]*"), "");
}

std::string chira::stripRight(const std::string& s) {
    return std::regex_replace(s, std::regex("[ |\t|\n|\r|\v|\f]*$"), "");
}

std::string chira::strip(const std::string& s) {
    return std::regex_replace(s, std::regex("^[ |\t|\n|\r|\v|\f]*|[ |\t|\n|\r|\v|\f]*$"), "");
}

std::string chira::stripLeft(const std::string& s, const std::string& c) {
    return std::regex_replace(s, std::regex("^[" + c +"]*"), "");
}

std::string chira::stripRight(const std::string& s, const std::string& c) {
    return std::regex_replace(s, std::regex("[" + c +"]*$"), "");
}

std::string chira::strip(const std::string& s, const std::string& c) {
    return std::regex_replace(s, std::regex("^[" + c + "]*|[" + c + "]*$"), "");
}

std::string chira::stripLeft(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("^[" + std::to_string(c) +"]*"), "");
}

std::string chira::stripRight(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("[" + std::to_string(c) +"]*$"), "");
}

std::string chira::strip(const std::string& s, char c) {
    return std::regex_replace(s, std::regex("^[" + std::to_string(c) + "]*|[" + c + "]*$"), "");
}

void chira::stripLeftModify(std::string& s) {
    s = stripLeft(s);
}

void chira::stripRightModify(std::string& s) {
    s = stripRight(s);
}

void chira::stripModify(std::string& s) {
    s = strip(s);
}

void chira::stripLeftModify(std::string& s, const std::string& c) {
    s = stripLeft(s, c);
}

void chira::stripRightModify(std::string& s, const std::string& c) {
    s = stripRight(s, c);
}

void chira::stripModify(std::string& s, const std::string& c) {
    s = strip(s, c);
}

void chira::stripLeftModify(std::string& s, char c) {
    s = stripLeft(s, c);
}

void chira::stripRightModify(std::string& s, char c) {
    s = stripRight(s, c);
}

void chira::stripModify(std::string& s, char c) {
    s = strip(s, c);
}
