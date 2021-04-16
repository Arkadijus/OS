#pragma once

#include <vector>
#include "VM.h"
#include "CPU.h"
#include "Memory.h"

class RM
{
private:
	CPU processor;
	Memory memory;

	std::vector<VM> programs;

	void parseCodeSegment(const std::string& command, Memory& memory, int location);
	void parseDataSegment(const std::string& dataSegmStr, Memory& memory);
	void fileToMemory(const std::string& filename, Memory& memory);

public:
	void StartProgram(const std::string& programFile);
	void RunAll();
};

