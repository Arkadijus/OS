#include "VM.h"
#include <iostream>
#include <string>

#define VMBLOCKS 16
#define VMWORDS 16

VM::VM(const std::vector<std::uint32_t>& programToRun, CPU& processor) : processor(processor)
{
	memory.WriteDataBlock(13, 0, programToRun);
}

// TODO: remove this
static std::string wordToString(std::uint32_t word)
{
	std::string strWord(4, (unsigned char)0);

	strWord[0] = (word >> 24) & 0xFF;
	strWord[1] = (word >> 16) & 0xFF;
	strWord[2] = (word >> 8) & 0xFF;
	strWord[3] = word & 0xFF;

	return strWord;
}

void VM::Run()
{
	while (true)
	{
		std::string instructionCode = wordToString(memory.GetWord(processor.IC++));

		if (instructionCode == "HALT")
			return;

		auto instrItr1 = VM::voidFunctions.find(instructionCode);
		if (instrItr1 != VM::voidFunctions.end())
		{
			//execute instr
			VM::voidFunc instr = instrItr1->second;
			(this->*instr)();
		}
		else
		{
			std::string instruction = instructionCode.substr(0, 2);
			std::string address = instructionCode.substr(2);

			int block = std::stoi(address.substr(0, 1), nullptr, 16); // TODO: TEMP
			int word = std::stoi(address.substr(1, 1), nullptr, 16);

			auto instrItr2 = VM::voidFunctionsWithAddress.find(instruction);
			if (instrItr2 != VM::voidFunctionsWithAddress.end())
			{
				//execute instr
				VM::voidFuncWithAddress instr = instrItr2->second;
				(this->*instr)(block, word);
			}
		}
	}
}

// Aritmetines
//------------------------------------------------
void VM::ADD()
{
	if (processor.AX > UINT_MAX - processor.BX)
		processor.SF |= CPU::StatusFlags::OF;
	processor.AX += processor.BX;
}

void VM::SUB()
{
	if (processor.AX > processor.BX)
		processor.SF |= CPU::StatusFlags::OF;
	processor.AX -= processor.BX;
}

void VM::MUL()
{
	if (processor.AX > UINT_MAX / processor.BX)
		processor.SF |= CPU::StatusFlags::OF;
	processor.AX *= processor.BX;
}

void VM::DIV()
{
	if (processor.BX != 0) // maybe exception here or smth or whatever 
		processor.AX /= processor.BX;
}
// Palyginimo
//------------------------------------------------
void VM::CMP()
{
	int tAX = processor.AX, tBX = processor.BX;
	tAX -= tBX;
	if (tAX == 0)
	{
		processor.SF |= CPU::StatusFlags::ZF;
		processor.SF &= ~CPU::StatusFlags::CF;
	}
	else if (tAX < 0)
	{
		processor.SF |= CPU::StatusFlags::CF;
		processor.SF &= ~CPU::StatusFlags::ZF;
	}
	else // tAX > tBX
	{
		processor.SF &= ~CPU::StatusFlags::CF;
		processor.SF &= ~CPU::StatusFlags::ZF;
	}
}

//	Duomenu
void VM::ReadToAX(std::uint32_t block, std::uint32_t word)
{
	processor.AX = memory.GetWord(block, word);
}

void VM::WriteFromAX(std::uint32_t block, std::uint32_t word)
{
	memory.WriteWord(block, word, processor.AX);
}

void VM::PrintAX()
{
	std::cout << processor.AX << std::endl;
}

void VM::PrintWord(std::uint32_t block, std::uint32_t word)
{
	std::cout << memory.GetWord(block, word) << std::endl;
}

void VM::WriteString(std::uint32_t block, std::uint32_t word)
{
	//TODO: doesnt work with integers
	std::vector<uint32_t> dataBlock; // TODO: unecessary copying

	while (true)
	{
		std::uint32_t data = memory.GetWord(processor.IC++);
		dataBlock.push_back(data);
		if (wordToString(data).find('$') != std::string::npos)
			break;
	}

	memory.WriteDataBlock(block, word, dataBlock);
}

void VM::PrintUntilEnd(std::uint32_t block, std::uint32_t word)
{
	memory.PrintUntilEnd(block, word);
}

void VM::InputAX()
{
	std::cin >> processor.AX;
}

void VM::InputWord(std::uint32_t block, std::uint32_t word)
{
	uint32_t input;
	std::cin >> input;
	memory.WriteWord(block, word, input);
}

void VM::Swap()
{
	uint32_t temp = processor.AX;
	processor.AX = processor.BX;
	processor.BX = temp;
}

void VM::Halt()
{
	//do who knows what
}

void VM::Jump(std::uint32_t block, std::uint32_t word)
{

	processor.IC = block * WORDCOUNT + word;
}

void VM::JumpMore(std::uint32_t block, std::uint32_t word)
{
	CMP();
	if (processor.SF & CPU::StatusFlags::ZF || processor.SF & CPU::StatusFlags::CF)
		return;
	Jump(block, word);
}

void VM::JumpLess(std::uint32_t block, std::uint32_t word)
{
	CMP();
	if (processor.SF & CPU::StatusFlags::CF)
		Jump(block, word);
}

void VM::JumpEqual(std::uint32_t block, std::uint32_t word)
{
	CMP();
	if (processor.SF & CPU::StatusFlags::ZF)
		Jump(block, word);
}

void VM::JumpNotEqual(std::uint32_t block, std::uint32_t word)
{
	CMP();
	if (processor.SF & CPU::StatusFlags::ZF)
		return;
	Jump(block, word);
}

void VM::JumpMoreEqual(std::uint32_t block, std::uint32_t word)
{
	CMP();
	if (processor.SF & CPU::StatusFlags::ZF || !(processor.SF & CPU::StatusFlags::CF))
		Jump(block, word);
}

void VM::JumpLessEqual(std::uint32_t block, std::uint32_t word)
{
	CMP();
	if (processor.SF & CPU::StatusFlags::ZF || processor.SF & CPU::StatusFlags::CF)
		Jump(block, word);
}

void VM::PrintMemory()
{
	memory.PrintMemory();
}

const std::unordered_map<std::string, VM::voidFunc> VM::voidFunctions =
{
	{"PRAX", &PrintAX},
	{"RDAX", &InputAX},
	{"SWAP", &Swap},
	{"_ADD", &ADD},
	{"_SUB", &SUB},
	{"_MUL", &MUL},
	{"_CMP", &CMP},
	{"HALT", &Halt}
};

const std::unordered_map<std::string, VM::voidFuncWithAddress> VM::voidFunctionsWithAddress =
{
	{"LA", &ReadToAX},
	{"SA", &WriteFromAX},
	{"PR", &PrintWord},
	{"PA", &PrintUntilEnd},
	{"SD", &WriteString},
	{"JP", &Jump},
	{"JM", &JumpMore},
	{"JL", &JumpLess},
	{"JE", &JumpEqual},
	{"JN", &JumpNotEqual},
	{"JX", &JumpMoreEqual},
	{"JY", &JumpLessEqual}
};
