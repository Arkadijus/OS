
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <thread>

#include "VM.h"
#include "RM.h"
#include "Process.h"
#include "Kernel.h"

bool isNumber(const std::string& line)
{
	if (line.empty())
		return false;

    auto start = line.begin();
    if (line[0] == '-' && line.size() > 1)
        start++;

	return std::all_of(start, line.end(), [](unsigned char c) { return std::isdigit(c); });
}

std::uint32_t StringToNumber(const std::string& str)
{
    // TODO: remove unsighed ints, or add a full support
    // need to handle negative numbers
    
    std::uint32_t num = 0;

    try
    {
        num = std::stoi(str);
        // Should possibly modify a status flag is str is negative, or save a bool flag
    }
    catch (const std::out_of_range&)
    {
        // If str has higher number than the INT32_MAX, it can still be lower than UINT32_MAX
        try
        {
            num = std::stoul(str);
        }
        catch (const std::out_of_range& e)
        {
            // handle numbers that can't fit into 4 bytes
            std::cout << "error: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "error: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }

    return num;
}

void interfaceFunc()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "Stop")
        {
            Element* ele = new Element;
            Kernel::getInstance().queue.push({ MOS_END, ele });
            break;
        }
        else
        {
            Element* ele = new Element;
            ele->string = new std::string(line);
            Kernel::getInstance().queue.push({ FROM_INTERFACE, ele });
        }
    }
}

int main()
{
    /*
    read file to VM memory code section (do validation while parsing)
    set IC to 0
    start VM
    read command shown by IC and execute
    */

    RM rm;
    ////rm.StartProgram("addNumbers.txt");
    //rm.StartProgram("addNumbers.txt");
    //rm.RunAll();

    Kernel::createInstance(&rm);
    StartStopProcess* startStop = new StartStopProcess(nullptr, ProcessState::Ready, 100);
    Process::createProcess(startStop, nullptr);

    std::thread interfaceThread(interfaceFunc);

    Kernel::getInstance().run();

    interfaceThread.join();
    return 0;
}