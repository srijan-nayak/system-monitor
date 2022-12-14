#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  explicit Process(int pid);
  [[nodiscard]] int Pid() const;
  [[nodiscard]] std::string User() const;
  [[nodiscard]] std::string Command() const;
  [[nodiscard]] float CpuUtilization() const;
  [[nodiscard]] std::string Ram() const;
  [[nodiscard]] long long int UpTime() const;
  bool operator<(Process const& other) const;

 private:
  int pid_;
};

#endif