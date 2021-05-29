#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Kernel.h"
#include "Resource.h"
#include "CPU.h"


struct SavedRegisters
{
	std::uint8_t SF = 0;
	std::uint32_t AX = 0;
	std::uint32_t BX = 0;
	std::uint32_t PTR = 0;
	std::uint16_t IC = 0;
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

class Process
{
public:
	Process(Process* parent, ProcessState initialState,
		std::uint8_t priority, const std::string& name);
	void saveRegisters();
	void restoreRegisters();

	virtual void run() = 0;

	static void createProcess(Process* process, Process* parent);
	static void destroyProcess(int ID);
	static void stopProcess(const std::string& name);
	static void activateProcess(const std::string& name);

	static void stopProcess(int ID);
	static void activateProcess(int ID);


	std::uint32_t getID() const { return m_ID; }
	ProcessState getState() const { return m_state; }
	void setState(ProcessState state);
	int getPriority() const { return m_priority; }
	void addElement(Element* element) { m_elementList.push_back(element); }
	void deleteElements(Resource* resource);
	std::vector<Resource*>& getCreatedResList() { return m_createdResList; }
	void deleteResource(int ID);
protected:

	std::uint32_t m_ID;
	std::string m_name;
	SavedRegisters m_savedRegisters;
	std::uint8_t m_priority;
	CPU* m_processor;
	ProcessState m_state;
	std::vector<Resource*> m_createdResList;
	std::vector<Element*> m_elementList;
	// owned elements
	Process* m_parent;
	std::vector<Process*> m_childProcesses;
	Kernel& m_kernel;
};

class StartStopProcess : public Process
{
public:
	StartStopProcess(Process* parent, ProcessState initialState,
		std::uint8_t priority)
		: Process(parent, initialState, priority, "StartStop") {}
	virtual void run() override;
};

class ReadFromInterfaceProcess : public Process
{
public:
	ReadFromInterfaceProcess(Process* parent, ProcessState initialState,
		std::uint8_t priority)
		: Process(parent, initialState, priority, "ReadFromInterface") {}
	virtual void run() override;
};

//class JCLProcess : public Process
//{
//public:
//	JCLProcess(Process* parent, ProcessState initialState,
//		std::uint8_t priority)
//		: Process(parent, initialState, priority, "JCL") {}
//	virtual void run() override;
//};

class MainProcProcess : public Process
{
public:
	MainProcProcess(Process* parent, ProcessState initialState,
		std::uint8_t priority)
		: Process(parent, initialState, priority, "MainProc") {}
	virtual void run() override;
};

class VirtualMachineProcess : public Process
{
public:
	VirtualMachineProcess(Process* parent, ProcessState initialState,
		std::uint8_t priority)
		: Process(parent, initialState, priority, "VirtualMachine") {}
	virtual void run() override;
};

class InterruptProcess : public Process
{
public:
	InterruptProcess(Process* parent, ProcessState initialState,
		std::uint8_t priority)
		: Process(parent, initialState, priority, "Interrupt") {}
	virtual void run() override;
};

class PrintLineProcess : public Process
{
public:
	PrintLineProcess(Process* parent, ProcessState initialState,
		std::uint8_t priority)
		: Process(parent, initialState, priority, "PrintLine") {}
	virtual void run() override;
};

class JobGovernorProcess : public Process
{
public:
	JobGovernorProcess(Process* parent, ProcessState initialState,
		std::uint8_t priority)
		: Process(parent, initialState, priority, "JobGovernor") {}
	virtual void run() override;
};



