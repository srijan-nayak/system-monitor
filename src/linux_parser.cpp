#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
  string os, kernel, version;
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
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float memTotal = 0;
  float memFree = 0;

  std::ifstream memInfoFile{LinuxParser::kProcDirectory +
                            LinuxParser::kMeminfoFilename};

  string line;
  string key;
  float value;
  if (memInfoFile.is_open()) {
    while (std::getline(memInfoFile, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream{line};
      lineStream >> key >> value;
      if (key == "MemTotal") memTotal = value;
      if (key == "MemFree") memFree = value;
      if (memTotal > 0 && memFree > 0) break;
    }
  }

  return (memTotal - memFree) / memTotal;
}

long LinuxParser::UpTime() {
  long upTime = 0;

  std::ifstream upTimeFile{LinuxParser::kProcDirectory +
                           LinuxParser::kUptimeFilename};

  string line;
  if (upTimeFile.is_open()) {
    std::getline(upTimeFile, line);
    std::istringstream lineStream{line};
    lineStream >> upTime;
  }
  return upTime;
}

long LinuxParser::Jiffies() {
  return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

long LinuxParser::ActiveJiffies() {
  long activeJiffies = 0;

  std::ifstream statFile{LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename};

  if (statFile.is_open()) {
    string line;
    std::getline(statFile, line);
    std::istringstream lineStream{line};
    string cpu;
    lineStream >> cpu;
    long value;
    while (lineStream >> value) {
      activeJiffies += value;
    }
  }
  return activeJiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() {
  int totalProcesses = 0;

  std::ifstream statFile{LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename};

  string line;
  string key;
  if (statFile.is_open()) {
    while (std::getline(statFile, line)) {
      std::istringstream lineStream{line};
      lineStream >> key;
      if (key == "processes") {
        lineStream >> totalProcesses;
        break;
      }
    }
  }
  return totalProcesses;
}

int LinuxParser::RunningProcesses() {
  int runningProcesses = 0;

  std::ifstream statFile{LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename};

  string line;
  string key;
  if (statFile.is_open()) {
    while (std::getline(statFile, line)) {
      std::istringstream lineStream{line};
      lineStream >> key;
      if (key == "procs_running") {
        lineStream >> runningProcesses;
        break;
      }
    }
  }
  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
