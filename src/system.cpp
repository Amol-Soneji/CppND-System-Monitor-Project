#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm> //For reversing the order of the proccess list.  

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
    for(unsigned int i = 0; i < pidVector.size(); i++)
    {
        Process p;
        p.setPid(pidVector[i]);
        float c {0};
        c = p.CpuUtilization() + c;//CpuUtilization() returns something.  
        string r {""};
        r = p.Ram();//Ram() returns something.  
        string u {""};
        u = p.User();//User() returns something.  
        long int up {0};
        up = p.UpTime() + up;//UpTime returns something.  
        string cmd {""};
        cmd = p.Command();//Command returns something.  
        if(i != 0)
        {
            processes_.push_back(p);
            int currentIndexP = processes_.size() - 1;
            for(long j = (processes_.size() - 1); j > -1; j--)
            {
                if(processes_[j] < p)
                {
                    std::swap(processes_[currentIndexP], processes_[j]);
                    currentIndexP = j;
                }
            }
        }
        else
            processes_.push_back(p);
    }
    std::reverse(processes_.begin(), processes_.end());
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