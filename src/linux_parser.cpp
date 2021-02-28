#include <dirent.h>
#include <unistd.h>
#include <experimental/filesystem>
#include <string>
#include <vector>

#include "linux_parser.h"

namespace fs = std::experimental::filesystem;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  string filename;
  fs::path proc_dir = fs::path(kProcDirectory);
  for (auto& p : fs::directory_iterator(proc_dir)) {
    if (fs::is_directory(p)) {
      filename = p.path().filename();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value, unit;
  int MemTotal{1}, MemFree{0};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value >> unit;
    if (key == "MemTotal:") {
      MemTotal = std::stoi(value, nullptr, 10);
    } else if (key == "MemFree:") {
      MemFree = std::stoi(value, nullptr, 10);
    } else {
      break;
    }
  }

  return ((float)(MemTotal - MemFree) / MemTotal);
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long uptime{0}, idletime{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

/*

Not Doing any "Jiffies" calculations in this project

// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }
*/

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  // I didn't use the CPU kStates becasue is seemed simpler to read the line
  // stream instead of converting to vector and using enum struct to access
  // indices.
  string cpuName, user, nice, system, idle, iowait, irq, softirq, steal,
      out_idle, non_idle, out_total;
  vector<string> returnVal;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  // only reading first line because currently only measureing aggregate CPU
  // total
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpuName >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal;
    break;
  }
  returnVal = {cpuName, user, nice, system, idle, iowait, irq, softirq, steal};
  return returnVal;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "processes") {
      return std::stoi(value, nullptr, 10);
    }
  }
  return 0;
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "procs_running") {
      return std::stoi(value, nullptr, 10);
    }
  }
  return 0;
}

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line, filename, other;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> filename >> other;
  }
  return filename;
}

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value, unit;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value >> unit;
    if (key == "VmSize:") {
      return to_string(std::stoi(value, nullptr, 10) / 1000);
    }
  }
  return string();
}

// Done: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value, other;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value >> other;
    if (key == "Uid:") {
      return value;
    }
  }
  return string();
}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);

  string line, str, username;
  std::regex searchExpr("(.):x:" + uid);
  std::regex unExpr(":x:");
  std::smatch name_match;

  std::ifstream stream(kPasswordPath);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> str;
    // Use REGEX to find correct username
    if (std::regex_search(str.begin(), str.end(), searchExpr)) {
      std::regex_search(str, name_match, unExpr);
      username = name_match.prefix();
      return username;
    }
  }

  return string();
}

// Done: Read and return the uptime of a process
// returns seconds after converting from clock ticks
//  sysconf(_SC_CLK_TCK)
long LinuxParser::UpTime(int pid) {
  string line;
  long uptime, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
      linestream.ignore(255, ' ');
    }
    linestream >> value;
    break;
  }
  uptime = value / sysconf(_SC_CLK_TCK);
  return uptime;
}

// Done: Read and Return a string vector containing the values needed to
// calculate the process CPU utilization
vector<string> LinuxParser::CpuUtilization(int PID) {
  string utime, stime, cutime, cstime;
  long starttime;
  vector<string> returnVal;
  string line;
  std::ifstream stream(kProcDirectory + to_string(PID) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; i++) {
      linestream.ignore(255, ' ');
    }
    linestream >> utime >> stime >> cutime >> cstime;
  }
  starttime = LinuxParser::UpTime(PID);
  returnVal = {utime, stime, cutime, cstime, to_string(starttime)};
  return returnVal;
}
