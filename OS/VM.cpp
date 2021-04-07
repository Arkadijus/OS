#include "VM.h"
#define VMBLOCKS 16
#define VMWORDS 16
VM::VM()
{
	memory.resize(VMBLOCKS * VMWORDS, 0); // resize to 1024 bytes with 0 as values
}
