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
	
	if (parentProcess) parentProcess->getCreatedResList().push_back(resource);

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
			{
				auto state = resources[i]->waitingProcesses[j].process->getState();
				if(state == ProcessState::Blocked)
					resources[i]->waitingProcesses[j].process->setState(ProcessState::Ready);
				else if (state == ProcessState::BlockedStopped)
					resources[i]->waitingProcesses[j].process->setState(ProcessState::ReadyStopped);
			}
			for (size_t j = 0; j < resources[i]->elements.size(); i++)
				delete resources[i]->elements[j];
			auto& processes = Kernel::getInstance().ProcessList;
			for (int j = 0; j < processes.size(); j++)
			{
				processes[j]->deleteElements(resources[i]);
			}

			if (resources[i]->parentProcess)
				resources[i]->parentProcess->deleteResource(resources[i]->id);

			delete resources[i];
			break;
		}
	}
}

void Resource::RequestResource(Process* requestingProcess, std::string name, int elementCount)
{
	auto& resources = Kernel::getInstance().ResourceList;
	bool found = false;
	requestingProcess->setState(ProcessState::Blocked); // stop it regardless of if there is such a resource or not
	for (size_t i = 0; i < resources.size(); i++)
	{
		if (resources[i]->name == name) // the resource we need
		{
			resources[i]->waitingProcesses.push_back(WaitingProcess(requestingProcess, elementCount));
			found = true;
			break;
		}
	}

	if (!found)
	{
		Resource::CreateResource(requestingProcess, name);
		Resource::RequestResource(requestingProcess, name, elementCount);
	}


	if (found)
		AssignResources();
}

void Resource::FreeResource(Process* parentProcess, Element* element, std::string name)
{
	auto& resources = Kernel::getInstance().ResourceList;
	bool found = false;
	for (size_t i = 0; i < resources.size(); i++)
	{
		if (resources[i]->name == name) // resource we need
		{
			element->resource = resources[i];
			resources[i]->elements.push_back(element);
			found = true;
		}
	}

	if (!found)
	{
		Resource::CreateResource(parentProcess, name);
		Resource::FreeResource(parentProcess, element, name);
	}

	if (found)
		AssignResources();
}

void Resource::AssignResources()
{
	// so I go trough each element in resources and check the processes that need them, choose based on priority
	auto& resources = Kernel::getInstance().ResourceList;
	for (int i = 0; i < resources.size(); i++)
	{
		int highestPriorityProcess = 0;
		int highestPriority = 0;

		if (resources[i]->waitingProcesses.empty())
			continue;

		for (int j = 0; j < resources[i]->waitingProcesses.size(); j++)
		{
			int tempPriority = resources[i]->waitingProcesses[j].process->getPriority();
			if (tempPriority > highestPriority)
			{
				highestPriority = tempPriority;
				highestPriorityProcess = j;
			}
		}
		// at this point we have the highest priority process for that resource, now we try to assign
		// just assign as many as we have or as many as needed

		while (resources[i]->waitingProcesses[highestPriorityProcess].elementCount > 0 && resources[i]->elements.size() > 0)
		{
			if (resources[i]->elements[0]->receiver == nullptr || resources[i]->elements[0]->receiver == resources[i]->waitingProcesses[highestPriorityProcess].process)
			{
				resources[i]->waitingProcesses[highestPriorityProcess].process->addElement(resources[i]->elements[0]);
				resources[i]->elements.erase(resources[i]->elements.begin());
				resources[i]->waitingProcesses[highestPriorityProcess].elementCount--;
			}
		}
		if (resources[i]->waitingProcesses[highestPriorityProcess].elementCount == 0)
		{
			auto state = resources[i]->waitingProcesses[highestPriorityProcess].process->getState();
			if (state == ProcessState::Blocked)
				resources[i]->waitingProcesses[highestPriorityProcess].process->setState(ProcessState::Ready);
			else if (state == ProcessState::BlockedStopped)
				resources[i]->waitingProcesses[highestPriorityProcess].process->setState(ProcessState::ReadyStopped);
		}

		// not sure what to do otherwise? Probably nothing but that leaves the question maybe to prioritize these elements that request the least elements first
	}

	Kernel::getInstance().runScheduler();
}