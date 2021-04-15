
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "VM.h"
#include "RM.h"

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

std::string wordToString(std::uint32_t word)
{
    std::string strWord(4, (unsigned char)0);

    strWord[0] = (word >> 24) & 0xFF;
    strWord[1] = (word >> 16) & 0xFF;
    strWord[2] = (word >> 8) & 0xFF;
    strWord[3] = word & 0xFF;

    return strWord;
}

std::vector<std::uint32_t> parseString(const std::string& line)
{
    // endline $
    std::vector<std::uint32_t> parsed;
    for (size_t i = 0; i < line.size(); i += 4)
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

bool isNumber(const std::string& line)
{
	if (line.empty())
		return false;

    auto start = line.begin();
    if (line[0] == '-' && line.size() > 1)
        start++;

	return std::all_of(start, line.end(), [](unsigned char c) { return std::isdigit(c); });
}

std::uint32_t StringToNumber(const std::string& str)
{
    // TODO: remove unsighed ints, or add a full support
    // need to handle negative numbers
    
    std::uint32_t num = 0;

    try
    {
        num = std::stoi(str);
        // Should possibly modify a status flag is str is negative, or save a bool flag
    }
    catch (const std::out_of_range&)
    {
        // If str has higher number than the INT32_MAX, it can still be lower than UINT32_MAX
        try
        {
            num = std::stoul(str);
        }
        catch (const std::out_of_range& e)
        {
            // handle numbers that can't fit into 4 bytes
            std::cout << "error: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "error: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }

    return num;
}

std::string removeWhitespace(const std::string& str)
{
    int leadingWs = str.find_first_not_of(' ');
    int trailingWs = str.find_last_not_of(' ');

    if (leadingWs == std::string::npos)
        return "";

    int count = trailingWs - leadingWs + 1;

    return str.substr(leadingWs, count);
}

std::vector<std::uint32_t> parseFile(const std::string& filename)
{
    std::ifstream fin(filename);
    std::vector<std::uint32_t> parsedCode;
    
    if (!fin.is_open())
    {
        //TODO: add error check/interrupts
        return {};
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

        line = removeWhitespace(line);

        if (line.size() < 4)
        {
            //TODO: add error check/interrupts. At this point parsing should fail, as all commands are at least 4 bytes
            continue;
        }

        std::string command = line.substr(0, 4);
        //TODO: check if commands accepts an arg, fail/interrupt otherwise
        std::string arg = line.size() > 4 ? line.substr(4) : "";

        //TODO: make sure that such command exists, also HALT command is required
        parsedCode.push_back(stringToWord(command));

        // remove whitespace before argument
        arg = removeWhitespace(arg);

        if (!arg.empty())
        {
            if (isString(arg))
            {
                std::vector<uint32_t> argBytes = parseString(arg);
                // here i assume that it's correct
                parsedCode.insert(parsedCode.end(), argBytes.begin(), argBytes.end());
            }
            else if (isNumber(arg))
            {
                // TODO: string to number can fail
                std::cout << StringToNumber(arg) << std::endl;
                parsedCode.push_back(StringToNumber(arg));
            }
            else
            {
                assert(false);
                //TODO: add error/interrupt for bad input
            }
        }
    }

    return parsedCode;
}

int main()
{
    /*
    read file to VM memory code section (do validation while parsing)
    set IC to 0
    start VM
    read command shown by IC and execute
    */

    auto parsedCode = parseFile("addNumbers.txt");
    RM rm;
    rm.CreateProcess(parsedCode);
    rm.RunAll();
    //VM vm(parsedCode);
    //vm.Run();

    return 0;
}