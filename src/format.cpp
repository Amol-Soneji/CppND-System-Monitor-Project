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
     int minutes = seconds / 60;
     int remainder = seconds % 60;
     string secondsString;
     if(remainder < 10)
          secondsString = "0" + remainder;
     else
          secondsString = remainder;
     int hours = minutes / 60;
     remainder = minutes % 60;
     string minuteString, hourString;
     if(remainder < 10)
          minuteString = "0" + remainder;
     else
          minuteString = remainder;
     if(hours < 10)
          hourString = "0" + hours;
     else
          hourString = hours;
     timeInHMS = hourString + ":" + minuteString + ":" + secondsString;
     return timeInHMS;
}