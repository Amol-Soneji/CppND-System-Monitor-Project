#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int seconds) //Had to switch this to long int as it was causing problems.  
{
     string timeInHMS;
     long int minutes = seconds / 60;
     long int remainder = seconds % 60;
     string secondsString;
     if(remainder < 10)
          secondsString = "0" + std::to_string(remainder);
     else
          secondsString = std::to_string(remainder);
     long int hours = minutes / 60;
     remainder = minutes % 60;
     string minuteString, hourString;
     if(remainder < 10)
          minuteString = "0" + std::to_string(remainder);
     else
          minuteString = std::to_string(remainder);
     if(hours < 10)
          hourString = "0" + std::to_string(hours);
     else
          hourString = std::to_string(hours);
     timeInHMS = hourString + ":" + minuteString + ":" + secondsString;
     return timeInHMS;
}