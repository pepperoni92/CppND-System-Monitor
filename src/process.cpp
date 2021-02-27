#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

void Process::Update() {
  user_ = LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
  cpu_ = LinuxParser::CpuUtilization(pid_);
  ram_ = LinuxParser::Ram(pid_);
  uptime_ = LinuxParser::UpTime(pid_);
}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_; }

// Return the command that generated this process
string Process::Command() const { return command_; }

// Return this process's memory utilization
string Process::Ram() const { return ram_; }

// Return the user (name) that generated this process
string Process::User() const { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }

// compare this Processes CPU Utilization against anothers
bool Process::operator<(Process const& a) const { return cpu_ > a.CpuUtilization(); }