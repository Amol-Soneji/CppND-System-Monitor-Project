#include "processor.h"
#include "linux_parser.h" //Added this to utilize linux_parser.  
#include <vector> //Had to add this, as the CPU utilization function in linux_parser returns a vector.  
#include <string> //Had to include this as the vector returned is a string.  

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return std::stof((LinuxParser::CpuUtilization())[0]); }