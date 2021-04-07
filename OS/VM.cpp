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
void VM::ReadToAX(int block, int word)
{
	processor.AX = memory.GetWord(block, word);
}

void VM::WriteFromAX(int block, int word)
{
	memory.WriteWord(block, word, processor.AX);
}

void VM::PrintAX()
{
	std::cout << processor.AX << std::endl;
}

void VM::PrintWord(int block, int word)
{
	memory.PrintWord(block, word);
}

int VM::WriteString(int block, int word, std::string str)
{
	return memory.WriteString(block, word, str);
}

void VM::PrintUntilEnd(int block, int word)
{
	memory.PrintUntilEnd(block, word);
}

void VM::InputAX()
{
	std::cin >> processor.AX;
}

int VM::InputWord(int block, int word)
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