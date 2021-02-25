#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor which sets private variable <pid_>
Process::Process(int p) : pid_(p) { cmdLine_ = LinuxParser::Command(p); }

// Test: Return this process's ID
int Process::Pid() {
  // return 0;
  return this->pid_;
  // cmdLine = LinuxParser::Command();
  // return 0;
}

// Test: Return this process's CPU utilization
float Process::CpuUtilization() {
  // return 0.0;
  vector<string> cpuInfo = LinuxParser::CpuUtilization(this->pid_);
  float uptime = LinuxParser::UpTime();
  float hertz = sysconf(_SC_CLK_TCK);
  float utime = std::stof(cpuInfo[0], nullptr) / hertz;
  float stime = std::stof(cpuInfo[1], nullptr) / hertz;
  float cutime = std::stof(cpuInfo[2], nullptr) / hertz;
  float cstime = std::stof(cpuInfo[3], nullptr) / hertz;
  float starttime = std::stof(cpuInfo[4], nullptr) / hertz;

  // std::cout << utime << stime << cutime << cstime << std::endl;

  float total_time = utime + stime;
  total_time +=
      cutime +
      cstime;  // Run this line if we want to include the children processes
  float seconds = uptime - starttime;

  // Calculate change in usage and time
  float delta_total_time = total_time - prev_total_time;
  float delta_seconds = seconds - prev_seconds;

  // Update running totals
  prev_total_time = total_time;
  prev_seconds = seconds;
  if (seconds == 0) {
    this->cpuUtil = 0;
  } else {
    this->cpuUtil = delta_total_time / delta_seconds;
  }

  return this->cpuUtil;
}

float Process::GetCpuUtil() { return this->cpuUtil; }

// Test: Return the command that generated this process
string Process::Command() {
  return cmdLine_;
  // return string();
}

// Test: Return this process's memory utilization
string Process::Ram() {
  return LinuxParser::Ram(this->pid_);
  // return string();
}

// Test: Return the user (name) that generated this process
string Process::User() {
  // return string();
  return LinuxParser::User(this->pid_);
}

// Test: Return the age of this process (in seconds)
long int Process::UpTime() {
  // return 0;
  long uptime = LinuxParser::UpTime(this->pid_);
  return uptime;
}

// Test: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  // return true;
  return (this->cpuUtil > a.cpuUtil);
}