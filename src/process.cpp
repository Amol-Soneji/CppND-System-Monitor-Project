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
int Process::Pid() { return 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() 
{
    string parsingLine;
    std::ifstream fileStream("/proc/" + std::to_string(PID) + "/stat");
    if(fileStream.is_open())
    {
        std::getline(fileStream, parsingLine);
        std::istringstream parserStream(parsingLine);
        to be done;
    }
}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
    if(command.compare("") == 0)
    {
        command = LinuxParser::Command(PID);
        return command;
    }
    else
        return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() 
{
    if(user.compare("") == 0)
    {
        user = LinuxParser::User(PID);
        return user;
    }
    else
        return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(PID); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }