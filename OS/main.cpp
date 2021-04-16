
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "VM.h"
#include "RM.h"

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

int main()
{
    /*
    read file to VM memory code section (do validation while parsing)
    set IC to 0
    start VM
    read command shown by IC and execute
    */

    RM rm;
    //rm.StartProgram("addNumbers.txt");
    rm.StartProgram("printLoop.txt");
    rm.RunAll();

    return 0;
}