#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  void Update();
  Processor& Cpu();                   
  std::vector<Process>& Processes(); 
  float MemoryUtilization() const;          
  long UpTime() const;                      
  int TotalProcesses() const;              
  int RunningProcesses() const;            
  std::string Kernel() const;               
  std::string OperatingSystem() const;    

 private: 
  void GetProcesses(); 
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  float memoryUtilized_ {0.0};
  std::string kernel_;
  std::string os_;
  long uptime_;
  int totalProcesses_;
  int runningProcesses_;
};

#endif