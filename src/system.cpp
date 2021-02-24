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
std::string System::Kernel() { return kernel_; }

// Return the system's memory utilization
float System::MemoryUtilization() { return memoryUtilized_; }

// Return the operating system name
std::string System::OperatingSystem() { return os_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return runningProcesses_; }

// Return the total number of processes on the system
int System::TotalProcesses() { return totalProcesses_; }

// Return the number of seconds since the system started running
long int System::UpTime() { return uptime_; }