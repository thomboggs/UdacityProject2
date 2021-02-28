#include <string>

#include "format.h"

using std::string;

// Done: Complete this helper function
string Format::ElapsedTime(long seconds) {
  long hours, minutes;
  string uptime{""}, seconds_str, minutes_str;
  hours = seconds / 3600;
  seconds -= hours * 3600;
  minutes = seconds / 60;
  seconds -= minutes * 60;

  if (minutes < 10) {
    minutes_str = '0' + std::to_string(minutes);
  } else {
    minutes_str = std::to_string(minutes);
  }

  if (seconds < 10) {
    seconds_str = '0' + std::to_string(seconds);
  } else {
    seconds_str = std::to_string(seconds);
  }

  uptime = std::to_string(hours) + ':' + minutes_str + ':' + seconds_str;
  return uptime;
}