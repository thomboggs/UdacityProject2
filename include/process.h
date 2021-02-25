#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int p);
  int Pid();               // TODO: See src/process.cpp
  std::string User();      // TODO: See src/process.cpp
  std::string Command();   // TODO: See src/process.cpp
  float CpuUtilization();  // TODO: See src/process.cpp
  float GetCpuUtil();
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  std::string cmdLine_{""};
  int pid_{0};
  float prev_total_time{0};
  float prev_seconds{0};
  float cpuUtil{0};
};

#endif