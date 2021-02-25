#include "processor.h"
#include <numeric>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;
using std::stoi;

// Done: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpuInfo = LinuxParser::CpuUtilization();
  vector<int> utilVals = {};
  for (unsigned i = 1; i < cpuInfo.size(); i++) {
    utilVals.push_back(std::stoi(cpuInfo[i], nullptr, 10));
  }
  int newIdle = utilVals[3] + utilVals[4];
  int newTotal = std::accumulate(utilVals.begin(), utilVals.end(),
                                 decltype(utilVals)::value_type(utilVals[0]));
  this->prevIdle = this->currIdle;
  this->prevTotal = this->currTotal;
  this->currIdle = newIdle;
  this->currTotal = newTotal;
  int deltaIdle = currIdle - prevIdle;
  int deltaTotal = currTotal - prevTotal;

  return (float)(deltaTotal - deltaIdle) / (float)deltaTotal;
}

void Processor::setName(string const cpuName) { this->name = cpuName; }
