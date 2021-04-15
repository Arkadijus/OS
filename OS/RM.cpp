#include "RM.h"

void RM::CreateProcess(std::vector<uint32_t> parsedCode)
{
	programs.push_back(VM(parsedCode, processor));
}

void RM::RunAll()
{
	for (int i = 0; i < programs.size(); i++)
	{
		programs[i].Run();
	}
}
