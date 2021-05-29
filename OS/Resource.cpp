#include "Resource.h"
#include "Process.h"
Resource::Resource(Process* parentProcess, std::string name)
{
	CreateResource(parentProcess, name);
}

Resource::~Resource()
{
	 
}

int Resource::CreateResource(Process* parentProcess, std::string name)
{
	Resource* resource = new Resource(parentProcess, name);
	resource->waitingProcesses.push_back((std::unique_ptr<Process>)parentProcess);
	resource->id = currentId;
	currentId++;
	resources.push_back(resource);
	return resource->id;
}

void Resource::DeleteResource(int id)
{
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->id == id)
		{
			for (int j = 0; j < resources[i]->waitingProcesses.size(); j++)
				Process::activateProcess(resources[i]->waitingProcesses[j]->getID());
			for (int j = 0; j < resources[i]->elements.size(); i++)
				delete resources[i]->elements[j];
			delete resources[i];
			break;
		}
	}
}

bool Resource::RequestResource(Process* requestingProcess, std::string name)
{
	bool ret = false;
	Process::stopProcess(requestingProcess->getID()); // stop it regardless of if there is such a resource or not
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->name == name) // the resource we need
		{
			resources[i]->waitingProcesses.push_back((std::unique_ptr<Process>)requestingProcess);
			ret = true;
		}
	}
	
	return ret;
}

void Resource::FreeResource(Element* element, std::string name)
{
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->name == name) // resource we need
			resources[i]->elements.push_back(element);
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