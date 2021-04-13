#pragma once
#include "CPU.h"
#include "Memory.h"
#include <vector>
#include <unordered_map>

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

	void ReadToAX(std::uint32_t block, std::uint32_t word);
	void WriteFromAX(std::uint32_t block, std::uint32_t word);
	void PrintAX();
	void PrintWord(std::uint32_t block, std::uint32_t word);
	int WriteString(std::uint32_t block, std::uint32_t word, std::string str);
	void PrintUntilEnd(std::uint32_t block, std::uint32_t word);
	void InputAX();
	int InputWord(std::uint32_t block, std::uint32_t word);
	void Swap();

	void PrintMemory();

	using voidFunc = void (VM::*)();
	using voidFuncWithAddress = void (VM::*)(std::uint32_t block, std::uint32_t word);
	static const std::unordered_map<std::string, voidFunc> voidFunctions;
	static const std::unordered_map<std::string, voidFuncWithAddress> voidFunctionsWithAddress;
};

