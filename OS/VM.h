#pragma once
#include "CPU.h"
#include "Memory.h"
#include <vector>

class VM
{
	CPU processor;
	Memory memory;
public:

	VM();

	void ADD();
	void SUB();
	void MUL();
	void DIV();

	void CMP();

	void ReadToAX(int block, int word);
	void WriteFromAX(int block, int word);
	void PrintAX();
	void PrintWord(int block, int word);
	int WriteString(int block, int word, std::string str);
	void PrintUntilEnd(int block, int word);
	void InputAX();
	int InputWord(int block, int word);
	void Swap();

	void PrintMemory();
};

