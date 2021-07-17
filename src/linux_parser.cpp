#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
      // Is every character of the name a digit?
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
float LinuxParser::MemoryUtilization() { return 0.0; }

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
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

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
  return std::stoi(value); //Although there will always be a "processes" in the stat file, c++ does not know this nor does it know what I am trying to do, and if the return statment here is not placed in the code, the compiler may complain.  
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
  return std::stoi(value); //Although there will always be a procs_running in the stat file, c++ does not know this nor does it know what I am trying to do, and if the return statment here is not placed in the code, the compiler may complain.  
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
    command = parsingLine;
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); to be done;}

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
      std::replace(parsingLine.begin(), parsingLine.end(), ":", " ");
      std::istringstream parseStream(parsingLine);
      while(parseStream >> key >> value) //I am only looking for the first value after UID key tag.  
      {
        if(key.compare("Uid") == 0)
          return value; //The first value after the row key is the UID I am looking for.  
      }
    }
  }
  return value; //Although there always is a line for UID in every status file, C++ does not know that or what I am trying to do, and the compiler will throw an error.  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{
  string UID = Uid(pid);
  string parsingLine, user, uValue;
  std::ifstream parseFileStream(kPasswordPath);
  if(parseFileStream.is_open())
  {
    while(std::getline(parseFileStream, parsingLine))
    {
      std::replace(parsingLine.begin(), parsingLine.end(), " ", "_");
      std::replace(parsingLine.begin(), parsingLine.end(), ":", " ");
      std::istringstream parseStream(parsingLine);
      while(parseStream >> user >> uValue)
      {
        if(uValue.compare(UID) == 0)
        {
          std::replace(user.begin(), user.end(), "_", " "); //Convert any spaces that were formerly in username that were converted into "_", back into spaces.  
          return user;
        }
      }
    }
  }
  return user; //Although there always will be a user for every UID that invoked a process, C++ does not know that or what I am trying to do, and the compiler will throw an error if this return statement does not exist.  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; to be done;}
