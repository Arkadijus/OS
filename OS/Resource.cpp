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
				resources[i]->waitingProcesses[j]->activateProcess();
			for (int j = 0; j < resources[i]->elements.size(); i++)
				delete resources[i]->elements[j];
			delete resources[i];
			break;
		}
	}
}

bool Resource::RequestResource(Process* requestingProcess, std::string name)
{
	requestingProcess->stopProcess(); // stop it regardless of if there is such a resource or not
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->name == name) // the resource we need
			resources[i]->waitingProcesses.push_back((std::unique_ptr<Process>)requestingProcess);
	}
	// resursu paskirstytojas?
	return true;
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