#include "Process.h"

#include <algorithm>
#include "Tools.h"

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


void Process::createProcess(Process* process, Process* parent, Element* element)
{
	Kernel& kernel = Kernel::getInstance();
	kernel.addToProcessList(process);
	if (process->getState() == ProcessState::Ready || process->getState() == ProcessState::ReadyStopped)
		kernel.addToReadyProcList(process);

	if (parent)
		parent->m_childProcesses.push_back(process);

	if (element)
		process->addElement(element);

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
	for (Resource* res : process->m_createdResList)
	{
		Resource::DeleteResource(res->id);
	}

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

Process* Process::getProcess(int ID)
{
	for (Process* proc : Kernel::getInstance().ProcessList)
	{
		if (proc->getID() == ID)
			return proc;
	}

	return nullptr;
}

Process* Process::getProcess(const std::string& name)
{
	for (Process* proc : Kernel::getInstance().ProcessList)
	{
		if (proc->getName() == name)
			return proc;
	}

	return nullptr;
}

void Process::stopProcess(int ID)
{
	Process* proc = Process::getProcess(ID);

	if (proc->getState() == ProcessState::Ready)
		proc->setState(ProcessState::ReadyStopped);
	else if (proc->getState() == ProcessState::Blocked)
		proc->setState(ProcessState::BlockedStopped);

	Kernel::getInstance().runScheduler();
}

void Process::activateProcess(int ID)
{
	Process* proc = Process::getProcess(ID);

	if (proc->getState() == ProcessState::ReadyStopped)
		proc->setState(ProcessState::Ready);
	else if (proc->getState() == ProcessState::BlockedStopped)
		proc->setState(ProcessState::Blocked);

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
		Resource::CreateResource(this, USER_MEMORY);
		
		for (int i = 0; i < 10; i++)
		{
			Element* elem = new Element;
			elem->mem = new Memory();
			Resource::FreeResource(this, elem, USER_MEMORY);
		}

		// initialize system processes
		Process* readFromInterface = new ReadFromInterfaceProcess(this, ProcessState::Ready, 95);
		Process::createProcess(readFromInterface, this);

		Process* mainProc = new MainProcProcess(this, ProcessState::Ready, 90);
		Process::createProcess(mainProc, this);


		Process* interrupt = new InterruptProcess(this, ProcessState::Ready, 90);
		Process::createProcess(interrupt, this);

		Process* idle = new IdleProcess(this, ProcessState::Ready, 0);
		Process::createProcess(idle, this);

		// block waiting for MOS end
		Resource::RequestResource(this, MOS_END, 1);
		return;
	}
	case 1:
	{
		m_processor->IC++;

		// delete system processes
		int i = m_childProcesses.size() - 1;
		for (; i >= 0; i--)
			Process::destroyProcess(m_childProcesses[i]->getID());

		// delete system resources
		i = m_createdResList.size() - 1;
		for (; i >= 0; i--)
			Resource::DeleteResource(m_createdResList[i]->id);

		Kernel::getInstance().RunningProc = nullptr;
	}
	default:
		break;
	}

}

void ReadFromInterfaceProcess::run()
{
	switch (m_processor->IC)
	{
	case 0:
		// Block for FromInterface resource
		for (int i = 0; i < m_elementList.size(); i++)
		{
			if (m_elementList[i]->resource->name == FROM_INTERFACE)
			{
				m_fileName = *m_elementList[i]->string;
				m_elementList.erase(m_elementList.begin() + i);
				m_processor->IC++;
				return;
			}
		}

		Resource::RequestResource(this, FROM_INTERFACE, 1);
		return;
	case 1:
		// Block for memory resource
		for (int i = 0; i < m_elementList.size(); i++)
		{
			if (m_elementList[i]->resource->name == USER_MEMORY)
			{
				m_memory = m_elementList[i]->mem;
				m_elementList.erase(m_elementList.begin() + i);
				m_processor->IC++;
				return;
			}
		}

		Resource::RequestResource(this, USER_MEMORY, 1);
		return;
	case 2:
		// Read file
		RM::fileToMemory(m_fileName, *m_memory);
		m_processor->IC++;
		return;
	case 3:
	{

		Element* elem = new Element;
		elem->mem = m_memory;
		elem->integer = 1;
		elem->sender = this;
		elem->receiver = Process::getProcess("MainProc");
		Resource::FreeResource(this, elem, PROGRAM_IN_MEMORY);

		m_fileName = "";
		m_memory = nullptr;
		m_processor->IC = 0;
		return;
	}
	default:
		return;
	}
}

void MainProcProcess::run()
{
	switch (m_processor->IC)
	{
	case 0:
		// Block for ProgramInMemory resource
		for (int i = 0; i < m_elementList.size(); i++)
		{
			if (m_elementList[i]->resource->name == PROGRAM_IN_MEMORY)
			{
				m_element = m_elementList[i];
				m_elementList.erase(m_elementList.begin() + i);
				m_processor->IC++;
				return;
			}
		}

		Resource::RequestResource(this, PROGRAM_IN_MEMORY, 1);
		return;
	case 1:
		// Create or delete VM

		if (m_element && m_element->integer)
		{
			Process* jobGov = new JobGovernorProcess(this, ProcessState::Ready, 85);
			Process::createProcess(jobGov, this, m_element);
		}
		else
		{
			if (m_element) Process::destroyProcess(m_element->sender->getID());
		}

		m_element = nullptr;

		m_processor->IC = 0;
	}
}

void JobGovernorProcess::run()
{
	// Create VM
	// Block for InterruptGov event
	// Stop VM
	// if Timer interrupt, decrease priority
	// if Halt interrupt destroy VM

	switch (m_processor->IC)
	{
	case 0:
	{
		for (int i = 0; i < m_elementList.size(); i++)
		{
			if (m_elementList[i]->resource->name == PROGRAM_IN_MEMORY)
			{
				m_element = m_elementList[i];
				m_elementList.erase(m_elementList.begin() + i);
			}
		}

		Process* vm = new VirtualMachineProcess(this, ProcessState::Ready, 80);
		Process::createProcess(vm, this, m_element);
		m_processor->IC++;
	}
	case 1:
		// Block for InterruptGov event
		for (int i = 0; i < m_elementList.size(); i++)
		{
			if (m_elementList[i]->resource->name == INTERRUPT_GOV)
			{
				m_interrupt = m_elementList[i]->string;
				m_elementList.erase(m_elementList.begin() + i);
				m_processor->IC++;
				return;
			}
		}

		Resource::RequestResource(this, INTERRUPT_GOV, 1);
		return;
	case 2:
		Process::stopProcess(m_childProcesses[0]->getID());

		if (*m_interrupt == "KILL")
		{
			Element* elem = new Element;
			elem->sender = this;
			elem->integer = 0;
			Resource::FreeResource(this, elem, PROGRAM_IN_MEMORY);
			Resource::RequestResource(this, "NonExistant", 1);
		}
		else
		{
			int priority = m_childProcesses[0]->getPriority();
			m_childProcesses[0]->setPriority(priority - 1);
			Process::activateProcess(m_childProcesses[0]->getID());
		}

		m_processor->IC = 0;
	}
}

void VirtualMachineProcess::run()
{
	m_processor->MODE = 1;
	
	for (int i = 0; i < m_elementList.size(); i++)
	{
		if (m_elementList[i]->resource->name == PROGRAM_IN_MEMORY)
		{
			m_element = m_elementList[i];
			m_elementList.erase(m_elementList.begin() + i);
			m_vm = new VM(*m_element->mem, m_processor);
		}
	}

	executeInstruction();
	if (test())
	{
		InterruptRegisters* registers = new InterruptRegisters;
		registers->PI = m_processor->PI;
		registers->TI = m_processor->TI;
		registers->SI = m_processor->SI;

		Element* elem = new Element;
		elem->sender = this;
		elem->receiver = Process::getProcess("Interrupt");
		elem->registers = registers;
		Resource::FreeResource(this, elem, INTERRUPT_VM);
	}
}

void VirtualMachineProcess::executeInstruction()
{
	Memory& memory = *m_element->mem;
	std::string instructionCode = Tools::wordToString(memory.GetWord(m_processor->IC++));

	auto instrItr1 = VM::voidFunctions.find(instructionCode);
	if (instrItr1 != VM::voidFunctions.end())
	{
		//execute instr
		VM::voidFunc instr = instrItr1->second;
		(m_vm->*instr)();
	}
	else
	{
		std::string instruction = instructionCode.substr(0, 2);
		std::string address = instructionCode.substr(2);

		int block = std::stoi(address.substr(0, 1), nullptr, 16); // TODO: TEMP
		int word = std::stoi(address.substr(1, 1), nullptr, 16);

		auto instrItr2 = VM::voidFunctionsWithAddress.find(instruction);
		if (instrItr2 != VM::voidFunctionsWithAddress.end())
		{
			//execute instr
			VM::voidFuncWithAddress instr = instrItr2->second;
			(m_vm->*instr)(block, word);
		}
	}
}

bool VirtualMachineProcess::test()
{
	return m_processor->TI == 0 || m_processor->SI != 0 || m_processor->PI != 0;
}

void InterruptProcess::run()
{
	// Block for Interrupt VM event
	// Identify interrupt
	// Set Job Governor
	// Send event

	switch (m_processor->IC)
	{
	case 0:
		for (int i = 0; i < m_elementList.size(); i++)
		{
			if (m_elementList[i]->resource->name == INTERRUPT_VM)
			{
				m_element = m_elementList[i];
				m_elementList.erase(m_elementList.begin() + i);
				m_processor->IC++;
				return;
			}
		}

		Resource::RequestResource(this, INTERRUPT_VM, 1);
		return;
	case 1:
	{

		auto registers = m_element->registers;
		Element* elem = new Element;
		elem->sender = this;
		elem->receiver = m_element->sender->getParent();

		if (registers->SI == 1) // HALT
		{
			elem->string = new std::string("KILL");
		}
		else if (registers->TI == 0) // TIMER
		{
			elem->string = new std::string("Timer");
		}

		Resource::FreeResource(this, elem, INTERRUPT_GOV);
		m_processor->IC = 0;
	}
	default:
		break;
	}
}

void Process::setState(ProcessState state)
{
	m_state = state;
	if (state == ProcessState::Blocked || state == ProcessState::BlockedStopped)
		Kernel::getInstance().addToBlockedProcList(this);
	else if (state == ProcessState::Ready || state == ProcessState::ReadyStopped)
		Kernel::getInstance().addToReadyProcList(this);

}

void Process::deleteElements(Resource* resource)
{
	m_elementList.erase(
		std::remove_if(m_elementList.begin(), m_elementList.end(),
			[resource](const Element* e) { return resource == e->resource; }),
			m_elementList.end());
}

void Process::deleteResource(int ID)
{
	for (int i = 0; i < m_createdResList.size(); i++)
	{
		if (m_createdResList[i]->id == ID)
		{
			m_createdResList.erase(m_createdResList.begin() + i);
			break;
		}
	}
}
