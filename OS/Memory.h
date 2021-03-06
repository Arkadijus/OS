#pragma once
#include <vector>
#include <string>

#define BLOCKCOUNT 16
#define WORDCOUNT 16

class Block
{
public:
	std::uint32_t words[WORDCOUNT];
};

class Memory
{
	std::vector<Block> blocks;
public:
	Memory();
	std::uint32_t GetWord(int blockNumber, int wordNumber);
	std::uint32_t GetWord(int address);
	std::string GetWordString(int blockNumber, int wordNumber);
	void WriteWord(int blockNumber, int wordNumber, std::uint32_t word);
	void WriteWord(int address, std::uint32_t word);
	void WriteDataBlock(int toBlock, int toWord, const std::vector<std::uint32_t>& dataBlock);
	void WriteDataBlock(int toAddress, const std::vector<std::uint32_t>& dataBlock);
	void PrintMemory();
	void PrintUntilEnd(int blockNumber, int wordNumber);
};

