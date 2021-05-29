#include "Kernel.h"

#include <algorithm>

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
        RunningProc->run();
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
        RunningProc = ReadyProcList[0];
        RunningProc->restoreRegisters();
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