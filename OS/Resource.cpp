#include "Resource.h"
#include "Process.h"

Resource::Resource(Process* parentProcess, std::string name)
	: parentProcess(parentProcess), name(name), kernel(Kernel::getInstance())
{
	id = currentId++;
}

Resource::~Resource()
{
	 
}

int Resource::CreateResource(Process* parentProcess, std::string name)
{
	Resource* resource = new Resource(parentProcess, name);
	Kernel::getInstance().ResourceList.push_back(resource);
	return resource->id;
}

void Resource::DeleteResource(int id)
{
	auto& resources = Kernel::getInstance().ResourceList;
	for (size_t i = 0; i < resources.size(); i++)
	{
		if (resources[i]->id == id)
		{
			for (size_t j = 0; j < resources[i]->waitingProcesses.size(); j++)
				Process::activateProcess(resources[i]->waitingProcesses[j]->getID());
			for (size_t j = 0; j < resources[i]->elements.size(); i++)
				delete resources[i]->elements[j];
			delete resources[i];
			break;
		}
	}
}

bool Resource::RequestResource(Process* requestingProcess, std::string name)
{
	auto& resources = Kernel::getInstance().ResourceList;
	bool found = false;
	requestingProcess->setState(ProcessState::Blocked); // stop it regardless of if there is such a resource or not
	for (size_t i = 0; i < resources.size(); i++)
	{
		if (resources[i]->name == name) // the resource we need
		{
			resources[i]->waitingProcesses.push_back(requestingProcess);
			found = true;
			break;
		}
	}

	if (!found)
	{
		Resource::CreateResource(requestingProcess, name);
		Resource::RequestResource(requestingProcess, name);
	}

	// resursu paskirstytojas?
	// planuotojas po paskirstytojo

	if (found)
		Kernel::getInstance().runScheduler();

	return true;
}

void Resource::FreeResource(Process* parentProcess, Element* element, std::string name)
{
	auto& resources = Kernel::getInstance().ResourceList;
	bool found = false;
	for (size_t i = 0; i < resources.size(); i++)
	{
		if (resources[i]->name == name) // resource we need
		{
			resources[i]->elements.push_back(element);
			found = true;
		}
	}

	if (!found)
	{
		Resource::CreateResource(parentProcess, name);
		Resource::FreeResource(parentProcess, element, name);
	}
	// resursu paskirsytojas
}

void Resource::AssignResources()
{
	// so I go trough each element in resources and check the processes that need them, choose based on priority
	for (int i = 0; i < resources.size(); i++)
	{
		int highestPriorityProcess = 0;
		int highestPriority = 0;
		for (int j = 0; j < resources[i]->waitingProcesses.size(); i++)
		{
			if(resources[i]->waitingProcesses[j]->)
		}
	}
}