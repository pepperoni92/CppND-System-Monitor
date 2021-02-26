#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

void System::Update() {
  GetProcesses();
  cpu_.UpdateUtilization();
  memoryUtilized_ = LinuxParser::MemoryUtilization();  
  kernel_ = LinuxParser::Kernel();
  os_ = LinuxParser::OperatingSystem();
  runningProcesses_ = LinuxParser::RunningProcesses();
  totalProcesses_ = LinuxParser::TotalProcesses();
  uptime_ = LinuxParser::UpTime();
}

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() const { return kernel_; }

// Return the system's memory utilization
float System::MemoryUtilization() const { return memoryUtilized_; }

// Return the operating system name
std::string System::OperatingSystem() const { return os_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() const { return runningProcesses_; }

// Return the total number of processes on the system
int System::TotalProcesses() const { return totalProcesses_; }

// Return the number of seconds since the system started running
long int System::UpTime() const { return uptime_; }

void System::GetProcesses() {
  processes_.clear();

  vector<int> pids = LinuxParser::Pids();
  
  for (int pid : pids) {
    Process proc(pid);
    proc.Update();
    processes_.push_back(proc);
  }

  std::sort(processes_.begin(), processes_.end());
}
