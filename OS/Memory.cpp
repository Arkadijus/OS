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
void Memory::WriteString(int blockNumber, int wordNumber, std::string str)
{
	// handle out of bounds and other stuff
	int index = 0;
	char c = str[index];
	while (c != '$')
	{
		char buffer[4];
		for (int i = 0; i < 4; i++)
		{
			index++;
			buffer[i] = str[index];
		}
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

void Memory::PrintWord(int blockNumber, int wordNumber)
{
	std::cout << blocks[blockNumber].words[wordNumber] << std::endl;
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
				bytes[0] = (unsigned char)blocks[i].words[j];
				bytes[1] = (unsigned char)(blocks[i].words[j] >> 8);
				bytes[2] = (unsigned char)(blocks[i].words[j] >> 16);
				bytes[3] = (unsigned char)(blocks[i].words[j] >> 24);

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
