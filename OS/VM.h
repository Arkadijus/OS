#pragma once
#include "CPU.h"
#include "Memory.h"
#include <vector>
#include <unordered_map>

class VM
{
	CPU processor;
	Memory memory;
	std::string inputBuffer;
public:

	VM(const std::vector<std::uint32_t>& programToRun);

	void Run();

	void ADD();
	void SUB();
	void MUL();
	void DIV();

	void CMP();

	void ReadToAX(std::uint32_t block, std::uint32_t word);
	void WriteFromAX(std::uint32_t block, std::uint32_t word);
	void PrintAX();
	void PrintWord(std::uint32_t block, std::uint32_t word);
	void WriteString(std::uint32_t block, std::uint32_t word);
	void PrintUntilEnd(std::uint32_t block, std::uint32_t word);
	void InputAX();
	void InputWord(std::uint32_t block, std::uint32_t word);
	void Swap();
	void Halt();

	void Jump(std::uint32_t block, std::uint32_t word);
	void JumpMore(std::uint32_t block, std::uint32_t word);
	void JumpLess(std::uint32_t block, std::uint32_t word);
	void JumpEqual(std::uint32_t block, std::uint32_t word);
	void JumpNotEqual(std::uint32_t block, std::uint32_t word);
	void JumpMoreEqual(std::uint32_t block, std::uint32_t word);
	void JumpLessEqual(std::uint32_t block, std::uint32_t word);

	void PrintMemory();

	using voidFunc = void (VM::*)();
	using voidFuncWithAddress = void (VM::*)(std::uint32_t block, std::uint32_t word);
	static const std::unordered_map<std::string, voidFunc> voidFunctions;
	static const std::unordered_map<std::string, voidFuncWithAddress> voidFunctionsWithAddress;
};

