#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  unsigned long long int prevIdle = 0;
  unsigned long long int prevTotal = 0;
};

#endif