#pragma once
#include "CPU.h"
#include <vector>

class VM
{
	CPU processor;
	std::vector<int> memory; // need someting like block class
public:

	VM();
};

