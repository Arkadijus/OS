#pragma once

#include <vector>
#include <memory>

#include "RM.h"

class Process;
class Resource;

class Kernel
{
public:
	Kernel(const Kernel&) = delete;
	void operator=(const Kernel&) = delete;

	static Kernel& getInstance();
	static void createInstance(RM* rm);


	std::vector<Process*> ProcessList;
	std::vector<Process*> ReadyProcList;
	std::vector<Process*> BlockedProcList;
	std::vector<Resource*> ResourceList;
	Process* RunningProc;
	RM* RealMachine;

	void run();
	void runScheduler();
	void addToProcessList(Process* proc);
	void addToReadyProcList(Process* proc);
	void addToBlockedProcList(Process* proc);
	void addToResourceList(Resource* proc);

private:
	Kernel(RM* rm) : RunningProc(nullptr), RealMachine(rm) {}

	static Kernel* instance;
};
