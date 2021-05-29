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


constexpr char MOS_END[] = "MOS end";
constexpr char USER_MEMORY[] = "User Memory";
constexpr char FROM_INTERFACE[] = "From Interface";

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
		delete string;
	}
};

struct WaitingProcess
{
	Process* process;
	int elementCount;
	WaitingProcess(Process* process, int elementCount) : process(process), elementCount(elementCount) {};
};

class Resource
{
	Process* parentProcess = nullptr;
	std::vector<WaitingProcess> waitingProcesses;
	static void AssignResources();
public:
	int id;
	std::string name;
	std::vector<Element*> elements;
	Kernel& kernel;

	Resource(Process* parentProcess, std::string name);
	~Resource();
	static int CreateResource(Process* parentProcess, std::string name);
	static void DeleteResource(int id);
	static void RequestResource(Process* requestingProcess, std::string name, int elementCount);
	static void FreeResource(Process* parentProcess, Element* element, std::string name);
};

static int currentId = 1;

