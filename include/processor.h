#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  long long int prevIdle = 0;
  long long int prevTotal = 0;
};

#endif