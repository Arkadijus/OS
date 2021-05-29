#pragma once

#include <vector>
#include "VM.h"
#include "CPU.h"
#include "Memory.h"

class RM
{
private:
	CPU processor;
	Memory memory;
	int executingProgram;

	std::vector<VM> programs;

	void parseCodeSegment(const std::string& command, Memory& memory, int location);
	void parseDataSegment(const std::string& dataSegmStr, Memory& memory);
	void fileToMemory(const std::string& filename, Memory& memory);

	// PI interrupts
	struct ProgramInterrupt
	{
		enum : std::uint8_t
		{
			invalidInstruction = 1,
			invalidAddress
		};
	};

	void invalidInstructionRoutine();
	void invalidAddressRoutine();

	// SI interrupts
	struct SupervisorInterrupt
	{
		enum : std::uint8_t
		{
			HALTInstruction = 1
		};
	};

	void HALTInstructionRoutine();

	bool interruptHappened() { return processor.SI != 0 || processor.PI != 0 || processor.TI == 0; }
	void executeInterruptRoutines();
	void handleInterrupts();

	bool isValidProgramIdx() { return executingProgram >= 0 && executingProgram < programs.size(); }

	void killExecutingVM();

public:
	RM() : executingProgram(-1) {}
	void StartProgram(const std::string& programFile);
	void RunAll();
	CPU* GetProcessor() { return &processor; }
};

