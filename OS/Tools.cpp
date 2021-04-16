#include "Tools.h"
#include <assert.h>

std::string Tools::removeWhitespace(const std::string& str)
{
    int leadingWs = str.find_first_not_of(' ');
    int trailingWs = str.find_last_not_of(' ');

    if (leadingWs == std::string::npos)
        return "";

    int count = trailingWs - leadingWs + 1;

    return str.substr(leadingWs, count);
}

std::string Tools::discardComment(const std::string& str)
{
    int pos = str.find("//");
    if (pos != std::string::npos)
        return str.substr(0, pos);
    return str;
}

std::string Tools::wordToString(std::uint32_t word)
{
    std::string strWord(4, (unsigned char)0);

    strWord[0] = (word >> 24) & 0xFF;
    strWord[1] = (word >> 16) & 0xFF;
    strWord[2] = (word >> 8) & 0xFF;
    strWord[3] = word & 0xFF;

    return strWord;
}

std::uint32_t Tools::stringToWord(const std::string& strWord)
{
    assert(strWord.size() <= 4);

    std::uint32_t word = 0;

    int toAdd = 4 - strWord.size();
    std::string paddedWord = strWord + std::string(toAdd, (unsigned char)0);

    word = std::uint32_t(
        (unsigned char)(paddedWord[0]) << 24 |
        (unsigned char)(paddedWord[1]) << 16 |
        (unsigned char)(paddedWord[2]) << 8 |
        (unsigned char)(paddedWord[3]));

    return word;
}

std::vector<std::uint32_t> Tools::parseString(const std::string& line)
{
    std::vector<std::uint32_t> parsed;
    for (size_t i = 0; i < line.size(); i += 4)
    {
        std::string word = line.substr(i, 4);
        parsed.push_back(stringToWord(word));
    }

    return parsed;
}

bool Tools::isString(const std::string& line)
{
    return line.rfind("$") != std::string::npos;
}
