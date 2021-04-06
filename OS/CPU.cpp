#include "CPU.h"
#include <climits>

// Aritmetines
//------------------------------------------------
void CPU::ADD()
{
	if (AX > UINT_MAX - BX)
		OF = true;
	AX += BX;
}

void CPU::SUB()
{
	if (AX > BX)
		OF = true;
	AX -= BX;
}

void CPU::MUL()
{
	if (AX > UINT_MAX / BX)
		OF = true;
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
		ZF = 1;
		CF = 0;
	}
	else if (tAX < tBX)
	{
		ZF = 0;
		CF = 1;
	}
	else // tAX > tBX
	{
		ZF = 0;
		CF = 0;
	}

}