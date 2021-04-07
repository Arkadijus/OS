#include "CPU.h"
#include <climits>

// Aritmetines
//------------------------------------------------
void CPU::ADD()
{
	if (AX > UINT_MAX - BX)
		SF |= StatusFlags::OF;
	AX += BX;
}

void CPU::SUB()
{
	if (AX > BX)
		SF |= StatusFlags::OF;
	AX -= BX;
}

void CPU::MUL()
{
	if (AX > UINT_MAX / BX)
		SF |= StatusFlags::OF;
	AX *= BX;
}

void CPU::DIV()
{
	if(BX != 0) // maybe exception here or smth or whatever 
		AX /= BX;
}
// Palyginimo
//------------------------------------------------
void CPU::CMP()
{
	unsigned int tAX = AX, tBX = BX;
	tAX -= tBX;
	if (tAX == tBX)
	{
		SF |= StatusFlags::ZF;
		SF &= ~StatusFlags::CF;
	}
	else if (tAX < tBX)
	{
		SF |= StatusFlags::CF;
		SF &= ~StatusFlags::ZF;
	}
	else // tAX > tBX
	{
		SF &= ~StatusFlags::CF;
		SF &= ~StatusFlags::ZF;
	}

}