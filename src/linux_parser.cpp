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

unsigned long long int LinuxParser::UpTime() {
  unsigned long long int upTime = 0;

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

unsigned long long int LinuxParser::Jiffies() {
  return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK);
}

unsigned long long int LinuxParser::ActiveJiffies(int pid) {
  unsigned long long int activeJiffies = 0;

  std::ifstream processStatFile{LinuxParser::kProcDirectory + to_string(pid) +
                                LinuxParser::kStatFilename};

  if (processStatFile.is_open()) {
    string line;
    std::getline(processStatFile, line);
    std::istringstream lineStream{line};

    string ignoredToken;
    unsigned long long int jiffies;
    // ignore all tokens except token 14-17 and add them to activeJiffies
    for (int i = 0; i < 17; ++i) {
      if (i < 13) {
        lineStream >> ignoredToken;
      } else {
        lineStream >> jiffies;
        activeJiffies += jiffies;
      }
    }
  }

  return activeJiffies;
}

unsigned long long LinuxParser::ActiveJiffies() {
  unsigned long long activeJiffies = 0;
  auto cpuStatValues = LinuxParser::CpuStatValues();
  for (size_t i = 0; i < cpuStatValues.size(); i++) {
    // guest and guestnice are already accounted in user and nice
    if (i != LinuxParser::CPUStates::kGuest_ &&
        i != LinuxParser::CPUStates::kGuestNice_) {
      activeJiffies += cpuStatValues[i];
    }
  }
  return activeJiffies;
}

unsigned long long int LinuxParser::IdleJiffies() {
  auto cpuStatValues = LinuxParser::CpuStatValues();
  auto idle = cpuStatValues[LinuxParser::CPUStates::kIdle_];
  auto iowait = cpuStatValues[LinuxParser::CPUStates::kIOwait_];
  return idle + iowait;
}

vector<unsigned long long int> LinuxParser::CpuStatValues() {
  vector<unsigned long long int> cpuStatValues{};

  std::ifstream statFile{LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename};

  if (statFile.is_open()) {
    string line;
    std::getline(statFile, line);
    std::istringstream lineStream{line};
    string cpu;
    unsigned long long int value;
    lineStream >> cpu;
    while (lineStream >> value) {
      cpuStatValues.emplace_back(value);
    }
  }
  return cpuStatValues;
}

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

string LinuxParser::Command(int pid) {
  string command;
  std::ifstream processCmdlineFile{LinuxParser::kProcDirectory +
                                   to_string(pid) +
                                   LinuxParser::kCmdlineFilename};
  if (processCmdlineFile.is_open()) {
    std::getline(processCmdlineFile, command);
  }
  return command;
}

string LinuxParser::Ram(int pid) {
  unsigned long long int ramUsage = 0;

  std::ifstream processStatusFile{LinuxParser::kProcDirectory + to_string(pid) +
                                  LinuxParser::kStatusFilename};

  if (processStatusFile.is_open()) {
    string line;
    string key;
    while (std::getline(processStatusFile, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream{line};
      lineStream >> key;
      // using VmRSS instead of VmSize for more accurate RAM usage
      if (key == "VmRSS") {
        lineStream >> ramUsage;
        break;
      }
    }
  }

  return to_string(ramUsage / 1024);
}

string LinuxParser::Uid(int pid) {
  string uid;

  std::ifstream processStatusFile{LinuxParser::kProcDirectory + to_string(pid) +
                                  LinuxParser::kStatusFilename};

  if (processStatusFile.is_open()) {
    string line;
    string key;
    while (std::getline(processStatusFile, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream{line};
      lineStream >> key;
      if (key == "Uid") {
        lineStream >> uid;
        break;
      }
    }
  }
  return uid;
}

string LinuxParser::User(int pid) {
  string user;

  string uid = LinuxParser::Uid(pid);
  std::ifstream passwdFile{LinuxParser::kPasswordPath};

  if (passwdFile.is_open()) {
    string line;
    string loginName, encryptedName, numericalUserId;
    while (std::getline(passwdFile, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream{line};
      lineStream >> loginName >> encryptedName >> numericalUserId;
      if (numericalUserId == uid) {
        user = loginName;
        break;
      }
    }
  }
  return user;
}

unsigned long long int LinuxParser::UpTime(int pid) {
  unsigned long long int upTime = 0;

  std::ifstream processStatFle{LinuxParser::kProcDirectory + to_string(pid) +
                               LinuxParser::kStatFilename};

  if (processStatFle.is_open()) {
    string line;
    std::getline(processStatFle, line);
    std::istringstream lineStream{line};

    // ignore the first 21 tokens
    string token;
    for (int i = 0; i < 21; ++i) {
      lineStream >> token;
    }

    unsigned long long int startTime;
    lineStream >> startTime;
    startTime /= sysconf(_SC_CLK_TCK);
    upTime = LinuxParser::UpTime() - startTime;
  }

  return upTime;
}
