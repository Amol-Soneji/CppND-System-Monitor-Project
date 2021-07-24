#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"//Added so I can utilize LinuxParser.  

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() 
{
    return PID;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{
    cpuUtil = 100 * (((float) LinuxParser::ActiveJiffies(Pid())) / ((float) LinuxParser::UpTime(Pid())));
    return cpuUtil;
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
    if(command.compare("") == 0)
    {
        command = LinuxParser::Command(Pid());
        return command;
    }
    else
        return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{
    RAM = LinuxParser::Ram(Pid());
    return RAM;
}

// TODO: Return the user (name) that generated this process
string Process::User() 
{
    if(user.compare("") == 0)
    {
        user = LinuxParser::User(Pid());
        return user;
    }
    else
        return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{
    Uptime = LinuxParser::UpTime(Pid());
    return Uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{
    
    if(a.cpuUtil < cpuUtil)
        return true;
    else
        return false;
}

//Added this setter class, as there needs to be some way to set the PID of a process.  
void Process::setPid(int p)
{
    PID = p;
}