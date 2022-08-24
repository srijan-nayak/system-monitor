#include "system.h"

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

Processor& System::Cpu() { return cpu_; }

std::vector<Process>& System::Processes() {
  for (const auto& pid : LinuxParser::Pids()) {
    processes_.emplace_back(Process(pid));
  }
  return processes_;
}

std::string System::Kernel() { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return operatingSystem_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long long int System::UpTime() { return LinuxParser::UpTime(); }
