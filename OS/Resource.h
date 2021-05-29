#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Kernel.h"

class Process;
class Resource;

// mos end -> sender->interface receiver->startstop
// task in supervizor mem -> stringu vektorius sender->read from interface receiver->JCL
// job in supervizor mem -> uint vectorius -> sender->JCL? sender->JOBGOVERNOR? receiver mainproc, struct uint vectoriu ir vykdymo laikas?
// interrupt event -> sender VirtualMachine receiver interrupt
// interrupt governor -> sender interrupt, receiver jobgovernor

struct Element
{
	Resource* resource = nullptr;
	Process* sender = nullptr;
	Process* receiver = nullptr;
	std::vector<std::string>* strings = nullptr;
	std::vector<uint32_t>* words = nullptr;
	std::string* string = nullptr;
	~Element()
	{
		delete strings;
		delete words;
	}
};

class Resource
{
	Process* parentProcess = nullptr;
	std::vector<Process*> waitingProcesses;
public:
	int id;
	std::string name;
	std::vector<Element*> elements;
	Kernel& kernel;

	Resource(Process* parentProcess, std::string name);
	~Resource();
	static int CreateResource(Process* parentProcess, std::string name);
	static void DeleteResource(int id);
	static bool RequestResource(Process* requestingProcess, std::string name);
	static void FreeResource(Process* parentProcess, Element* element, std::string name);
};

static int currentId = 1;

