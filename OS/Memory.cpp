#include "Memory.h"
#include <iostream>

Memory::Memory()
{
	blocks.resize(BLOCKCOUNT);
}
std::uint32_t Memory::GetWord(int blockNumber, int wordNumber)
{
	if (blockNumber < BLOCKCOUNT && wordNumber < WORDCOUNT)
		return blocks[blockNumber].words[wordNumber];
	//else error
}
std::uint32_t Memory::GetWord(int address)
{
	int block = address / WORDCOUNT;
	int word = address % WORDCOUNT;
	
	return GetWord(block, word);
}
std::string Memory::GetWordString(int blockNumber, int wordNumber)
{
	if (blockNumber < BLOCKCOUNT && wordNumber < WORDCOUNT)
		return std::to_string(blocks[blockNumber].words[wordNumber]);
	//else error
}
void Memory::WriteWord(int blockNumber, int wordNumber, uint32_t word)
{
	if (blockNumber < BLOCKCOUNT && wordNumber < WORDCOUNT)
		blocks[blockNumber].words[wordNumber] = word;
	// else error
}

void Memory::WriteDataBlock(int toBlock, int toWord, const std::vector<std::uint32_t>& dataBlock)
{
	if (toBlock < 0 || toWord < 0)
		return; // add out of bounds interrupt/error

	if (toBlock * 16 + toWord + dataBlock.size() > BLOCKCOUNT * WORDCOUNT)
		return; // add out of bounds interrupt/error

	for (int i = 0; i < dataBlock.size(); i++)
	{
		int block = toBlock + i / WORDCOUNT;
		int word = toWord + i % WORDCOUNT;
		blocks[block].words[word] = dataBlock[i];
	}
}

void Memory::PrintMemory()
{
	for (int i = 0; i < BLOCKCOUNT; i++)
	{
		for (int j = 0; j < WORDCOUNT; j++)
		{
			std::cout << std::to_string(blocks[i].words[j]);
		}
		std::cout << std::endl;
	}
}


void Memory::PrintUntilEnd(int blockNumber, int wordNumber)
{
	if (blockNumber < BLOCKCOUNT && wordNumber < WORDCOUNT)
	{
		int i = blockNumber;
		int j = wordNumber;
		unsigned char bytes[4];
		while (i < BLOCKCOUNT)
		{
			while (j < WORDCOUNT)
			{
				std::uint32_t word = blocks[i].words[j];

				bytes[0] = (word >> 24) & 0xFF;
				bytes[1] = (word >> 16) & 0xFF;
				bytes[2] = (word >> 8) & 0xFF;
				bytes[3] = word & 0xFF;

				for (int index = 0; index < 4; index++)
				{
					if (bytes[index] != '$')
						std::cout << bytes[index];
					else
					{
						std::cout << std::endl;
						return;
					}
				}
				j++;
			}
			i++;
			j = 0;
		}
	}

}

uint32_t Memory::GetWord(int blockNumber, int wordNumber)
{
	return blocks[blockNumber].words[wordNumber];
}
