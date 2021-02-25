#include <string>

#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  //   Processor(std::vector<std::string>){};
  float Utilization();  // Done: See src/processor.cpp
  void setName(std::string const cpuName);
  // TODO: Declare any necessary private members
 private:
  int prevIdle{0};
  int prevTotal{0};
  int currIdle{0};
  int currTotal{0};
  std::string name{""};
};

#endif