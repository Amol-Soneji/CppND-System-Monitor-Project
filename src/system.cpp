#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"  //Added this to utilize LinuxParser.  

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() 
{
    cpu_.Utilization();
    return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() 
{
    vector<int> pidVector = LinuxParser::Pids();
    for(int i = 0; i < pidVector.size(); i++)
    {
        Process p;
        p.setPid(pidVector[i]);
        float c = p.CpuUtilization();
        string r = p.Ram();
        string u = p.User();
        long int up = p.UpTime();
        string cmd = p.Command();
        if(i != 0)
        {
            processes_.push_back(p);
            for(long j = (processes_.size() - 1); j > -1; j--)
            {
                if(processes_[j] < p)
                    std::swap(processes_[(processes_.size() - 1)], processes_[j]);
            }
        }
        else
            processes_.push_back(p);
    }
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }