#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  explicit Process(int pid);
  [[nodiscard]] int Pid() const;
  [[nodiscard]] std::string User() const;
  [[nodiscard]] std::string Command() const;
  float CpuUtilization();                  // TODO: See src/process.cpp
  [[nodiscard]] std::string Ram() const;
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  const int pid_;
};

#endif