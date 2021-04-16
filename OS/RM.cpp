#include "RM.h"
#include <fstream>
#include <iostream>
#include <assert.h>

#include "Tools.h"

void RM::parseCodeSegment(const std::string& command, Memory& memory, int location)
{
    if (command.size() != 4)
    {
        // TODO: handle error?
    }

    memory.WriteWord(location, Tools::stringToWord(command));
}

void RM::parseDataSegment(const std::string& dataSegmStr, Memory& memory)
{
    std::string address = dataSegmStr.substr(0, 4);
    int toBlock, toWord;

    try
    {
        toBlock = std::stoi(address.substr(2, 1), nullptr, 16);
        toWord = std::stoi(address.substr(3, 1), nullptr, 16);
    }
    catch (const std::exception& e)
    {
        // TODO: handle
        std::cout << "error: " << e.what() << std::endl;
        return;
    }

    std::string data = dataSegmStr.size() > 4 ? dataSegmStr.substr(4) : "";

    // remove whitespace before data
    data = Tools::removeWhitespace(data);

    if (!data.empty())
    {
        if (Tools::isString(data))
        {
            std::vector<uint32_t> dataBytes = Tools::parseString(data);
            memory.WriteDataBlock(toBlock, toWord, dataBytes);
        }
        else
        {
            assert(false);
            //TODO: add error/interrupt for bad input
        }
    }
}

void RM::fileToMemory(const std::string& filename, Memory& memory)
{
    std::ifstream fin(filename);
    std::vector<std::uint32_t> parsedCode;
    int location = 0;

    enum class Segment
    {
        CodeSegment,
        DataSegment
    };

    Segment segment = Segment::CodeSegment;


    if (!fin.is_open())
    {
        //TODO: add error check/interrupts
        return;
    }

    std::string line;
    while (std::getline(fin, line))
    {
        line = Tools::discardComment(line);
        line = Tools::removeWhitespace(line);

        if (line.empty())
        {
            continue;
        }
        else if (line.size() < 4)
        {
            //TODO: add error check/interrupts. At this point parsing should fail, as all commands are at least 4 bytes
            continue;
        }

        if (line == ".code")
        {
            segment = Segment::CodeSegment;
            continue;
        }
        else if (line == ".data")
        {
            segment = Segment::DataSegment;
            continue;
        }

        if (segment == Segment::CodeSegment)
            parseCodeSegment(line, memory, location++);
        else if (segment == Segment::DataSegment)
            parseDataSegment(line, memory);
    }
}

void RM::StartProgram(const std::string& programFile)
{
	// give memory to VM
    fileToMemory(programFile, memory);
    programs.push_back(VM(memory, processor));
}

void RM::RunAll()
{
	for (size_t i = 0; i < programs.size(); i++)
	{
		programs[i].Run();
	}
}
