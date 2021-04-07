
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <assert.h>

std::uint32_t stringToWord(const std::string& strWord)
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

std::vector<std::uint32_t> parseString(const std::string& line)
{
    // endline $
    std::vector<std::uint32_t> parsed;
    for (int i = 0; i < line.size(); i += 4)
    {
        std::string word = line.substr(i, 4);
        parsed.push_back(stringToWord(word));
    }

    return parsed;
}

bool isString(const std::string& line)
{
    return line.rfind("$") != std::string::npos;
}

void parseFile(const std::string& filename)
{
    std::ifstream fin(filename);
    
    if (!fin.is_open())
    {
        //error
        return;
    }

    std::string line;
    while (std::getline(fin, line))
    {
        // discard comments
        int pos = line.find("//");
        if (pos != std::string::npos)
        {
            line = line.substr(0, pos);
        }

        if (line.size() < 4)
        {
            //to short
            continue;
        }

        std::string command = line.substr(0, 4); // check if string is long enough

        std::string arg = line.size() > 4 ? line.substr(4) : "";

        // remove whitespace before argument
        pos = arg.find_first_not_of(' ');
        arg = arg.substr(pos == std::string::npos ? 0 : pos);

        if (!arg.empty())
        {
            if (isString(arg))
            {
                auto a = parseString(arg);
            }
            else
            {

            }
        }
    }

}

int main()
{
    parseFile("addNumbers.txt");
    return 0;
}