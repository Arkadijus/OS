#pragma once

#include <string>
#include <vector>

class Tools
{
public:
	static std::string removeWhitespace(const std::string& str);
	static std::string discardComment(const std::string& str);
	static std::string wordToString(std::uint32_t word);
	static std::uint32_t stringToWord(const std::string& strWord);
	static std::vector<std::uint32_t> parseString(const std::string& line);
	static bool isString(const std::string& line);
};

