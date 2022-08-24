#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  static float MemoryUtilization();
  static long long int UpTime();
  static int TotalProcesses();
  static int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

 private:
  Processor cpu_;
  std::vector<Process> processes_;
  std::string operatingSystem_{LinuxParser::OperatingSystem()};
  std::string kernel_{LinuxParser::Kernel()};
};

#endif