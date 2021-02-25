#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  void UpdateUtilization();

 private:
   float currentUtilization_ {0.0};
   float prevActualIdle_ {0.0};
   float prevNonIdle_ {0.0};
};

#endif