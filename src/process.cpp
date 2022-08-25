#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() const {
  auto activeJiffies = LinuxParser::ActiveJiffies(pid_);
  auto activeSeconds = activeJiffies / sysconf(_SC_CLK_TCK);
  auto elapsedSeconds = LinuxParser::UpTime(pid_);
  return ((float)activeSeconds / (float)elapsedSeconds);
}

string Process::Command() const { return LinuxParser::Command(pid_); }

string Process::Ram() const { return LinuxParser::Ram(pid_); }

string Process::User() const { return LinuxParser::User(pid_); }

long long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& other) const {
  return this->CpuUtilization() < other.CpuUtilization();
}