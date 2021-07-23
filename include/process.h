#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  void setPid(int p); //Added this setter class, as there needs to be some way to set the PID of a process.  

  // TODO: Declare any necessary private members
 private:
    int PID {0}; //Set PID to default of 0.  
    float cpuUtil {00.0}; //Set cpuUtil to default of 00.0.  
    long int Uptime {0}; //Set Uptime to default of 0.  
    std::string RAM{""};//Set RAM to default of blank string.  
    std::string user {""}; //Set user to default of a blank string.  
    std::string command {""}; //Set command to default of a blank string.
};

#endif