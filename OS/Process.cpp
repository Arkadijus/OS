#include "Process.h"

#include <algorithm>

static int currentID = 1;

Process::Process(const Process* parent, ProcessState initialState, std::uint8_t priority,
	const std::string& name)
{
	m_ID = currentID++;
}

void Process::saveRegisters()
{
	m_savedRegisters.SF = m_processor->SF;
	m_savedRegisters.AX = m_processor->AX;
	m_savedRegisters.BX = m_processor->BX;
	m_savedRegisters.PTR = m_processor->PTR;
	m_savedRegisters.IC = m_processor->IC;
}

void Process::restoreRegisters()
{
	m_processor->SF = m_savedRegisters.SF;
	m_processor->AX = m_savedRegisters.AX;
	m_processor->BX = m_savedRegisters.BX;
	m_processor->PTR = m_savedRegisters.PTR;
	m_processor->IC = m_savedRegisters.IC;
}


void Process::createProcess(std::unique_ptr<Process>&& process, Process* parent)
{
	// change unique ptr location
	ProcessList.push_back(process);
	parent->m_childProcesses.push_back(process.get());
}

void Process::destroyProcess(int ID)
{
	int toDelete = -1;
	Process* process;
	for (int i = 0; i < ProcessList.size(); i++)
	{
		if (ProcessList[i]->getID() == ID)
		{
			process = ProcessList[i].get();
			toDelete = i;
			break;
		}
	}

	if (!process)
		return;

	// delete resources
	for (Process* proc : process->m_childProcesses)
		Process::destroyProcess(proc->getID());

	if (process->m_parent)
	{
		auto& parentList = process->m_parent->m_childProcesses;
		std::remove(parentList.begin(), parentList.end(), process);
	}

	ProcessList.erase(ProcessList.begin() + toDelete);
}

void Process::stopProcess(const std::string& name)
{
}

void Process::activateProcess(const std::string& name)
{
}

void StartStopProcess::run()
{
	// initialize system resources
	// supervisor memory
	// user memory
	// initialize system processes
	std::unique_ptr<Process> readFromInterface(new ReadFromInterfaceProcess(this, ProcessState::Ready, 90));
	Process::createProcess(std::move(readFromInterface), this);

	std::unique_ptr<Process> JCL(new JCLProcess(this, ProcessState::Ready, 90));
	Process::createProcess(std::move(JCL), this);

	std::unique_ptr<Process> mainProc(new MainProcProcess(this, ProcessState::Ready, 90));
	Process::createProcess(std::move(mainProc), this);

	std::unique_ptr<Process> interrupt(new InterruptProcess(this, ProcessState::Ready, 90));
	Process::createProcess(std::move(interrupt), this);

	// block waiting for MOS end
	// 
	// request MOS end
	// 
	// delete system processes
	
	for (Process* process : m_childProcesses)
		Process::destroyProcess(process->getID());

	// delete system resources
}

void ReadFromInterfaceProcess::run()
{
	// Block for FromInterface resource
	// Read file
	// Block for memory resource
	// copy file into memory
	// Send TaskInMemory


	
}

void MainProcProcess::run()
{
}

void VirtualMachineProcess::run()
{
}

void InterruptProcess::run()
{
}

void PrintLineProcess::run()
{
}

void JobGovernorProcess::run()
{
}

void JCLProcess::run()
{
}
