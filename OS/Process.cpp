#include "Process.h"

#include <algorithm>

static int currentID = 1;

Process::Process(Process* parent, ProcessState initialState, std::uint8_t priority,
	const std::string& name) : m_parent(parent), m_state(initialState), m_priority(priority), m_name(name), m_processor(nullptr),
	m_kernel(Kernel::getInstance())
{
	m_processor = m_kernel.RealMachine->GetProcessor();
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


void Process::createProcess(Process* process, Process* parent)
{
	Kernel& kernel = Kernel::getInstance();
	kernel.addToProcessList(process);
	if (process->getState() == ProcessState::Ready || process->getState() == ProcessState::ReadyStopped)
		kernel.addToReadyProcList(process);

	if (parent)
		parent->m_childProcesses.push_back(process);

	kernel.runScheduler();
}

void Process::destroyProcess(int ID)
{
	auto& ProcessList = Kernel::getInstance().ProcessList;
	int toDelete = -1;
	Process* process = nullptr;
	for (int i = 0; i < ProcessList.size(); i++)
	{
		if (ProcessList[i]->getID() == ID)
		{
			process = ProcessList[i];
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
		parentList.erase(std::remove(parentList.begin(), parentList.end(), process), parentList.end());
	}

	ProcessList.erase(ProcessList.begin() + toDelete);
	Kernel::getInstance().runScheduler();
}

void Process::stopProcess(const std::string& name)
{
	Kernel::getInstance().runScheduler();
}

void Process::activateProcess(const std::string& name)
{
	Kernel::getInstance().runScheduler();
}

void Process::stopProcess(int ID)
{
	Kernel::getInstance().runScheduler();
}

void Process::activateProcess(int ID)
{
	Kernel::getInstance().runScheduler();
}

void StartStopProcess::run()
{
	switch (m_processor->IC)
	{
	case 0:
	{

		m_processor->IC++;
		// initialize system resources
		Resource::CreateResource(this, "UserMemory");

		// initialize system processes
		Process* readFromInterface = new ReadFromInterfaceProcess(this, ProcessState::Ready, 90);
		Process::createProcess(readFromInterface, this);

		/*Process* JCL = new JCLProcess(this, ProcessState::Ready, 90);
		Process::createProcess(JCL), this);*/

		Process* mainProc = new MainProcProcess(this, ProcessState::Ready, 90);
		Process::createProcess(mainProc, this);

		Process* interrupt = new InterruptProcess(this, ProcessState::Ready, 90);
		Process::createProcess(interrupt, this);

		// block waiting for MOS end
		Resource::RequestResource(this, "MOS end");
		return;
	}
	case 1:
	{

		m_processor->IC++;
		// request MOS end
		// 
		// delete system processes
	
		int i = m_childProcesses.size() - 1;
		for (; i >= 0; i--)
			Process::destroyProcess(m_childProcesses[i]->getID());

		Kernel::getInstance().RunningProc = nullptr;

	}
	default:
		break;
	}

	// delete system resources
}

void ReadFromInterfaceProcess::run()
{
	switch (m_processor->IC)
	{
	case 0:
		// Block for FromInterface resource
		m_processor->IC++;
		Resource::RequestResource(this, "FromInterface");
		for (auto elem : m_elementList)
		{
			if (elem->resource->name == "FromInterface")
				continue;
		}

		break;
	case 1:
		// Read file
		m_processor->IC++;
		break;
	case 2:
		// Block for memory resource
		m_processor->IC++;
		Resource::RequestResource(this, "UserMemory");
		break;
	case 3:
		// copy file into memory
		// Send TaskInMemory
		m_processor->IC = 0;
		break;
	default:
		break;
	}
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

//void JCLProcess::run()
//{
//}
void Process::deleteElements(Resource* resource)
{
	for (int i = 0; i < m_elementList.size(); i++)
	{
		if (m_elementList[i]->resource == resource)
			m_elementList.erase(m_elementList.begin() + i);
	}
}