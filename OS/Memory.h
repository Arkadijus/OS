#pragma once
#include <vector>
#include <string>
//#include "Block.h"
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
	std::string GetWordString(int blockNumber, int wordNumber);
	int WriteUINT32(int blockNumber, int wordNumber, uint32_t word);
	int WriteString(int blockNumber, int wordNumber, std::string str);
	void PrintMemory();
};

