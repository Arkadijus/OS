#pragma once
#include <vector>
#include "VM.h"
#include "CPU.h"
class RM
{
	std::vector<VM> programs;
	CPU processor;
public:
	void CreateProcess(std::vector<uint32_t> parsedCode);
	void RunAll();
};

