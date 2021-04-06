#pragma once
#include <climits>;
class CPU
{
	bool OF = false;
	bool CF = false;
	bool ZF = false;
	
	bool MODE = false;

	unsigned int AX = 0;
	unsigned int BX = 0;
	
	unsigned int PTR[4];

	unsigned int TI = 100;
	unsigned int PI = 0;
	unsigned int SI = 0;
	//unsigned int IC??

public:
	void ADD();
	void SUB();
	void MUL();
	void DIV();

	void CMP();
};

