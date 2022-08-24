#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() {
  auto total = LinuxParser::ActiveJiffies();
  auto idle = LinuxParser::IdleJiffies();

  auto deltaTotal = total - prevTotal;
  auto deltaIdle = idle - prevIdle;

  prevTotal = total;
  prevIdle = idle;

  return (float)(std::abs(deltaTotal - deltaIdle)) / (float)deltaTotal;
}