#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>//For debugging.  

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::cout;//For debugging.  

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  string parsingLine, totalMem, freeMem, tag, value;
  float usedMem;
  std::ifstream parseFileStream(kProcDirectory + kMeminfoFilename);
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parsingLine))
    {
      std::replace(parsingLine.begin(), parsingLine.end(), ':', ' ');
      std::istringstream parseStream(parsingLine);
      while(parseStream >> tag >> value)
      {
        if(tag.compare("MemTotal") == 0)
          totalMem = value;
        else if(tag.compare("MemFree") == 0)
          freeMem = value;
      }
    }
  }
  usedMem = std::stof(totalMem) - std::stof(freeMem);
  return (usedMem / std::stof(totalMem));
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  string parsingLine;
  string uptimeString;
  std::ifstream parseFileStream(kProcDirectory + kUptimeFilename);
  if(parseFileStream.is_open())
  {
    std::getline(parseFileStream, parsingLine);
    std::istringstream parseStream(parsingLine);
    parseStream >> uptimeString;//I will use the first value, as I am looking for the total time system is powered on.  
  }
  return std::stol(uptimeString);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { //cout << "Looking at Jiffies() \n";
  return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) //Accordin to man7.org for the man page of "/proc" the variables involved in the calc already have their clockticks divided by _SC_CLK_TCK.  
{
  string parseLine, a, b, c, d, e, f, g, h, i, j, k, l, m, userTime, sysTime, childUserTime, childSysTime;
  std::ifstream parseFileStream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(parseFileStream.is_open())
  {
    std::getline(parseFileStream, parseLine);
    std::istringstream parseStream(parseLine);
    parseStream >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> k >> l >> m >> userTime >> sysTime >> childUserTime >> childSysTime;
    return ((std::stol(userTime) / sysconf(_SC_CLK_TCK)) + (std::stol(sysTime) / sysconf(_SC_CLK_TCK)));//+ (std::stol(childUserTime)) + (std::stol(childSysTime)));//Unable to find reason for why it goes to negative sometimes.  
  }
  return 0; //Compiler will return a warning because it does not know that the if statment above will mostly always be true.  
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  string parseLine, cpuTag, userTag, niceTag, systemTag, idleTag, ioWaitTag, irqTag, softIRQTag, stealTag;
  long activeTUnits;
  std::ifstream parseFileStream(kProcDirectory + kStatFilename);
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parseLine))
    {
      std::istringstream parseStream(parseLine);
      while(parseStream >> cpuTag >> userTag >> niceTag >> systemTag >> idleTag >> ioWaitTag >> irqTag >> softIRQTag >> stealTag)
      {
        if(cpuTag.compare("cpu"))
        {
          activeTUnits = std::stol(userTag) + std::stol(niceTag) + std::stol(systemTag) + std::stol(irqTag) + std::stol(softIRQTag) + std::stol(stealTag);
          return activeTUnits;
        }
      }
    }
  }
  return activeTUnits; //Although there will always be a line for "cpu" in the stat file, the compiler does not know this nore what I am trying to do, if I do not add a return statment here, the compiler may complain.  
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  string parseLine, cpuTag, userTag, niceTag, systemTag, ildeTag, ioWaitTag;
  long idleTUnits;
  std::ifstream parseFileStream(kProcDirectory + kStatFilename);
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parseLine))
    {
      std::istringstream parseStream(parseLine);
      while(parseStream >> cpuTag >> userTag >> niceTag >> systemTag >> ildeTag >> ioWaitTag)
      {
        idleTUnits = std::stol(ildeTag) + std::stol(ioWaitTag);
        return idleTUnits;
      }
    }
  }
  return idleTUnits; //Although there will always be a line for "cpu" in the stat file, the compiler does not know this nore what I am trying to do, if I do not add a return statment here, the compiler may complain.  
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{
  vector<string> utilization(1);
  utilization[0] = std::to_string(((float)ActiveJiffies()) / ((float)Jiffies()));
  return utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  string parsingline, tag, value;
  std::ifstream parseFileStream(kProcDirectory + kStatFilename);
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parsingline))
    {
      std::istringstream parseStream(parsingline);
      while(parseStream >> tag >> value)
      {
        if(tag.compare("processes") == 0)
          return std::stoi(value);
      }
    }
  }
  return std::stoi(value); //Although there will always be a "processes" in the stat file, the compiler does not know this nor does it know what I am trying to do, and if the return statment here is not placed in the code, the compiler may complain.  
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{
  string parsingLine, tag, value;
  std::ifstream parseFileStream(kProcDirectory + kStatFilename);
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parsingLine))
    {
      std::istringstream parseStream(parsingLine);
      while(parseStream >> tag >> value)
      {
        if(tag.compare("procs_running") == 0)
          return std::stoi(value);
      }
    }
  }
  return std::stoi(value); //Although there will always be a procs_running in the stat file, the compiler does not know this nor does it know what I am trying to do, and if the return statment here is not placed in the code, the compiler may complain.  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  string parsingLine, command;
  std::ifstream parseFileStream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(parseFileStream.is_open())
  {
    std::getline(parseFileStream, parsingLine);
    if((parsingLine.compare("") == 0) || (parsingLine.compare(" ") == 0))//Some proccesses may not display anything.  
      command = "N/A";
    else
      command = parsingLine;
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
  string parsingLine, usage, tag, value;//a, b, c, d, e, f, g, h, i, j, k, l, m, userTime, sysTime, childUserTime, childSysTime, n, o, p, q, startTime, vsize;
  std::ifstream parseFileStream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  //std::ifstream parseFileStream(kProcDirectory + std::to_string(pid) + kStatFilename);//Checking if one of the fields in stat file could substitute.  field 23.
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parsingLine))
    {
      std::replace(parsingLine.begin(), parsingLine.end(), ':', ' ');
      std::istringstream parseStream(parsingLine);
      while(parseStream >> tag >> value)
      {
        if(tag.compare("VmRSS") == 0) // The VmRSS field represents the memory usage of the process.  
        {
          
          int usageInMB = std::stoi(value) / 1024;
          usage = std::to_string(usageInMB);
          return usage;
        }
      } 

    }
    return "0";//Some system proccesses may not display ram usage. //In stat file those proccesses have 0 in field 23.  
    //std::getline(parseFileStream, parsingLine);
    //std::istringstream parseStream(parsingLine);
    //parseStream >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> k >> l >> m >> userTime >> sysTime >> childUserTime >> childSysTime >> n >> o >> p >> q >> startTime >>vsize;
    //return std::to_string(std::stol(vsize) / 1000000);
  }
  return std::to_string(0); //There are instances on some flavors of Linux where VmSize is not listed in the status file of a process.  An example would be a system task running on Kali Linux.  
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
  string parsingLine, key, value;
  std::ifstream parseFileStream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parsingLine))
    {
      std::replace(parsingLine.begin(), parsingLine.end(), ':', ' ');
      std::istringstream parseStream(parsingLine);
      while(parseStream >> key >> value) //I am only looking for the first value after UID key tag.  
      {
        if(key.compare("Uid") == 0)
          return value; //The first value after the row key is the UID I am looking for.  
      }
    }
  }
  return value; //Although there always is a line for UID in every status file, the compiler does not know that or what I am trying to do, and the compiler will throw an error.  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{
  string UID = Uid(pid);
  string parsingLine, user, otherValue, uValue;
  std::ifstream parseFileStream(kPasswordPath);
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parsingLine))
    {
      std::replace(parsingLine.begin(), parsingLine.end(), ' ', '_');
      std::replace(parsingLine.begin(), parsingLine.end(), ':', ' ');
      std::istringstream parseStream(parsingLine);
      while(parseStream >> user >> otherValue >> uValue)
      {
        if(uValue.compare(UID) == 0)
        {
          std::replace(user.begin(), user.end(), '_', ' '); //Convert any spaces that were formerly in username that were converted into "_", back into spaces.  
          return user;
        }
      }
    }
  }
  return user; //Although there always will be a user for every UID that invoked a process, the compiler does not know that or what I am trying to do, and the compiler will throw an error if this return statement does not exist.  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{
  string versionToParse = Kernel();
  string bigRelease, release, subRelease;
  std::replace(versionToParse.begin(), versionToParse.end(), '.', ' ');
  std::istringstream versStreamParse(versionToParse);
  versStreamParse >> bigRelease >> release >> subRelease;
  //if(((std::stoi(bigRelease)) > 2) || ((std::stoi(bigRelease) == 2) && (std::stoi(release) > 5))) //No need to do diving by clockticks in Linux version 2.6 and later, according to man page documentation.  
  //{
  //  string parsingLine, a, b, c, d, e, f, g, h, i, j, k, l, m, userTime, sysTime, childUserTime, childSysTime, n, o, p, q, startTime;
  //  std::ifstream parseFileStream(kProcDirectory + std::to_string(pid) + kStatFilename);
  //  if(parseFileStream.is_open())
  //  {
  //    std::getline(parseFileStream, parsingLine);
  //    std::istringstream parseStream(parsingLine);
  //    parseStream >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> k >> l >> m >> userTime >> sysTime >> childUserTime >> childSysTime >> n >> o >> p >> q >> startTime;
  //    if(((UpTime() - (std::stol(startTime))) < 0) && ((UpTime() - (std::stol(startTime) / sysconf(_SC_CLK_TCK))) >= 0))
  //    {
  //      cout << pid << "\n";
  //      cout << "Maybe I am wrong.  \n";//For Debugging.  
  //    }
  //    return (UpTime() - (std::stol(startTime)));
  //  }
  //}
  //else
  //{  For some odd reason it seems linux may not be dividing by _SC_CLK_TCK.  
    string parsingLine, a, b, c, d, e, f, g, h, i, j, k, l, m, userTime, sysTime, childUserTime, childSysTime, n, o, p, q, startTime;
    std::ifstream parseFileStream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if(parseFileStream.is_open())
    {
      std::getline(parseFileStream, parsingLine);
      std::istringstream parseStream(parsingLine);
      parseStream >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j >> k >> l >> m >> userTime >> sysTime >> childUserTime >> childSysTime >> n >> o >> p >> q >> startTime;
      return (UpTime() - ((std::stol(startTime)) / sysconf(_SC_CLK_TCK)));
    }
  return 0; //Compiler does not know the if statment above will mostly always be true.  
  //}
}
