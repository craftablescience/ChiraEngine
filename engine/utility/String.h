#pragma once

#include <string>
#include <vector>

namespace chira::String {

bool startsWith(const std::string& str, char beginning);
bool startsWith(const std::string& str, const std::string& beginning);

bool endsWith(const std::string& str, char end);
bool endsWith(const std::string& str, const std::string& end);

bool contains(const std::string& str, char sub);
bool contains(const std::string& str, const std::string& substr);

void remove(std::string& input, char charToRemove);

std::vector<std::string> split(const std::string& input, char delimiter);

std::string stripLeft(const std::string& s);
std::string stripRight(const std::string& s);
std::string strip(const std::string& s);

std::string stripLeft(const std::string& s, const std::string& c);
std::string stripRight(const std::string& s, const std::string& c);
std::string strip(const std::string& s, const std::string& c);

std::string stripLeft(const std::string& s, char c);
std::string stripRight(const std::string& s, char c);
std::string strip(const std::string& s, char c);

} // namespace chira::String
