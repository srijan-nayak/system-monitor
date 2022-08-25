#include "system.h"

#include <algorithm>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

Processor& System::Cpu() { return cpu_; }

std::vector<Process>& System::Processes() {
  processes_.clear();
  for (const auto& pid : LinuxParser::Pids()) {
    processes_.emplace_back(Process(pid));
  }
  std::sort(processes_.begin(), processes_.end(), [](Process a, Process b) {
    return b < a;
  });
  return processes_;
}

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return operatingSystem_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long long int System::UpTime() { return LinuxParser::UpTime(); }
