#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Done: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> const active_pids = LinuxParser::Pids();
  // loop through pids list and create a process for each and
  //  return vector of the processes
  // Need to read which processes are in the processes_ list and
  //  remove ones which aren't in pids and not double create ones that are in
  //  pids

  // Make a vector of ints of pids in processes_ vector

  vector<int> current_pids = {};
  for (Process p : processes_) {
    // std::cout << p.Pid() << std::endl;
    current_pids.push_back(p.Pid());
  }

  for (int pid : active_pids) {
    // Is PID In processes_?
    // If so ignore
    // If not create process
    // found how to use std::find from stack overflow
    // std::cout << pid << std::endl;
    auto result = std::find(current_pids.begin(), current_pids.end(), pid);

    if (result == current_pids.end()) {
      Process proc = Process(pid);
      processes_.push_back(proc);
    }
  }

  int this_pid;
  // Check if current_pids is not in active_pids
  for (unsigned int i = 0; i < processes_.size(); i++) {
    this_pid = processes_[i].Pid();
    if (std::find(active_pids.begin(), active_pids.end(), this_pid) ==
        active_pids.end()) {
      processes_.erase(processes_.begin() + i + 1);
    } else {
      // need to update cpu utilization here so it can be correctly used to sort
      // the list
      processes_[i].CpuUtilization();
    }
  }

  // Sort the vector by cpu utilization
  std::sort(processes_.begin(), processes_.end());

  return processes_;
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() {
  if (kernel == "") {
    kernel = LinuxParser::Kernel();
  }
  return kernel;
}

// Done: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Done: Return the operating system name
std::string System::OperatingSystem() {
  if (OSname == "") {
    OSname = LinuxParser::OperatingSystem();
  }
  return OSname;
}

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Done: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Done: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }