#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "Resource.h"

struct SavedRegisters
{
	std::uint8_t SF;
	std::uint32_t AX;
	std::uint32_t BX;
	std::uint32_t PTR;
	std::uint16_t IC;
};

enum class ProcessState
{
	Running,
	Ready,
	Blocked,
	Stopped,
	BlockedStopped,
	ReadyStopped
};

class CPU;

struct Process
{
	// Process list pointer (ready/blocked)
	std::uint32_t ID;
	std::string name;
	SavedRegisters savedRegisters;
	std::uint8_t priority;
	CPU* processor;
	ProcessState state;
	std::vector<Resource> resourceList;
	Process* father;
	std::vector<Process*> childProcesses;

	void saveRegisters();
	void restoreRegisters();

	static void createProcess(const Process* parent, ProcessState initialState, );
	static void destroyProcess();
	static void stopProcess();
	static void activateProcess();
};





