#include "VM.h"
#include <iostream>
#define VMBLOCKS 16
#define VMWORDS 16

VM::VM()
{
	
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
	unsigned int tAX = processor.AX, tBX = processor.BX;
	tAX -= tBX;
	if (tAX == tBX)
	{
		processor.SF |= CPU::StatusFlags::ZF;
		processor.SF &= ~CPU::StatusFlags::CF;
	}
	else if (tAX < tBX)
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
	memory.PrintWord(block, word);
}

int VM::WriteString(std::uint32_t block, std::uint32_t word, std::string str)
{
	return memory.WriteString(block, word, str);
}

void VM::PrintUntilEnd(std::uint32_t block, std::uint32_t word)
{
	memory.PrintUntilEnd(block, word);
}

void VM::InputAX()
{
	std::cin >> processor.AX;
}

int VM::InputWord(std::uint32_t block, std::uint32_t word)
{
	uint32_t input;
	std::cin >> input;
	return memory.WriteWord(block, word, input);
}

void VM::Swap()
{
	uint32_t temp = processor.AX;
	processor.AX = processor.BX;
	processor.BX = temp;
}

void VM::PrintMemory()
{
	memory.PrintMemory();
}

const std::unordered_map<std::string, VM::voidFunc> VM::voidFunctions =
{
	{"RDAX", &CMP},
	{"SWAP", &Swap},
	{"PRAX", &CMP},
	{"HALT", &CMP},
};

const std::unordered_map<std::string, VM::voidFuncWithAddress> VM::voidFunctionsWithAddress =
{
	{"SD", &PrintUntilEnd},
	{"PA", &PrintUntilEnd},
};
