#include "Kernel.h"

#include <algorithm>
#include <iostream>

#include "Process.h"

Kernel* Kernel::instance = nullptr;

Kernel& Kernel::getInstance()
{
    if (!instance)
        instance = new Kernel(nullptr);

    return *instance;
}

void Kernel::createInstance(RM* rm)
{
    if (!instance)
        instance = new Kernel(rm);
}

void Kernel::run()
{
    while (RunningProc && !ProcessList.empty())
    {
        if (!queue.isEmpty())
        {
            auto elem = queue.pop();
            Resource::FreeResource(nullptr, elem.elem, elem.name);
        }
        RunningProc->run();
    }
}

void Kernel::runScheduler()
{
    if (RunningProc)
    {
        if (RunningProc->getState() == ProcessState::Blocked || RunningProc->getState() == ProcessState::BlockedStopped)
            addToBlockedProcList(RunningProc);
        else
            addToReadyProcList(RunningProc);

        RunningProc->saveRegisters();
    }


    if (!ReadyProcList.empty())
    {
        for (Process* proc : ReadyProcList)
        {
            if (proc->getState() != ProcessState::ReadyStopped)
            {
                RunningProc = proc;
                std::cout << "Selected Proc ID: " << RunningProc->getID() << std::endl;
                RunningProc->restoreRegisters();
                break;
            }
        }
    }
}

void Kernel::addToProcessList(Process* proc)
{
    if (std::find_if(ProcessList.begin(), ProcessList.end(), [proc](Process* p) { return p->getID() == proc->getID(); }) == ProcessList.end())
        ProcessList.push_back(proc);
}

void Kernel::addToReadyProcList(Process* proc)
{
    addToProcessList(proc);

    BlockedProcList.erase(
        std::remove_if(BlockedProcList.begin(), BlockedProcList.end(),
            [proc](const Process* p) { return p->getID() == proc->getID(); }),
        BlockedProcList.end());

    if (std::find_if(ReadyProcList.begin(), ReadyProcList.end(), [proc](Process* p) { return p->getID() == proc->getID(); }) == ReadyProcList.end())
        ReadyProcList.push_back(proc);

    std::sort(ReadyProcList.begin(), ReadyProcList.end(),
        [](const Process* a, const Process* b) { return a->getPriority() > b->getPriority(); });
}

void Kernel::addToBlockedProcList(Process* proc)
{
    addToProcessList(proc);

    ReadyProcList.erase(
        std::remove_if(ReadyProcList.begin(), ReadyProcList.end(),
            [proc](const Process* p) { return p->getID() == proc->getID(); }),
        ReadyProcList.end());

    if (std::find_if(BlockedProcList.begin(), BlockedProcList.end(), [proc](Process* p) { return p->getID() == proc->getID(); }) == BlockedProcList.end())
        BlockedProcList.push_back(proc);
}

void Kernel::addToResourceList(Resource* proc)
{
}

void Kernel::deleteProcess(int ID)
{
    ProcessList.erase(
        std::remove_if(ProcessList.begin(), ProcessList.end(),
            [ID](const Process* p) { return p->getID() == ID; }),
        ProcessList.end());

    ReadyProcList.erase(
        std::remove_if(ReadyProcList.begin(), ReadyProcList.end(),
            [ID](const Process* p) { return p->getID() == ID; }),
        ReadyProcList.end());

    BlockedProcList.erase(
        std::remove_if(BlockedProcList.begin(), BlockedProcList.end(),
            [ID](const Process* p) { return p->getID() == ID; }),
        BlockedProcList.end());
}

bool Queue::isEmpty()
{
    std::unique_lock<std::mutex> lock(mutex);
    return queue.empty();
}

QueueElem Queue::pop()
{
    std::unique_lock<std::mutex> lock(mutex);
    auto elem = queue.front();
    queue.pop();
    return elem;
}

void Queue::push(const QueueElem& elem)
{
    std::unique_lock<std::mutex> lock(mutex);
    queue.push(elem);
}
