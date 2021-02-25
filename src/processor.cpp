#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

// Return the aggregate CPU utilization
void Processor::UpdateUtilization() { 
  vector<string> cpuStrings = LinuxParser::CpuUtilization();

  if (cpuStrings.size() < 8) return;

  float user = stof(cpuStrings[LinuxParser::CPUStates::kUser_]);
  float nice = stof(cpuStrings[LinuxParser::CPUStates::kNice_]);
  float system = stof(cpuStrings[LinuxParser::CPUStates::kSystem_]);
  float idle = stof(cpuStrings[LinuxParser::CPUStates::kIdle_]);
  float iowait = stof(cpuStrings[LinuxParser::CPUStates::kIOwait_]);
  float irq = stof(cpuStrings[LinuxParser::CPUStates::kIRQ_]);
  float softirq = stof(cpuStrings[LinuxParser::CPUStates::kSoftIRQ_]);
  float steal = stof(cpuStrings[LinuxParser::CPUStates::kSteal_]);

  float actualIdle = idle + iowait;
  float nonIdle = user + nice + system + irq + softirq + steal;
  
  float prevTotal = prevActualIdle_ + prevNonIdle_;
  float total = actualIdle + nonIdle;

  float totalDelta = total - prevTotal;
  float idleDelta = actualIdle - prevActualIdle_;

  currentUtilization_ = (totalDelta - idleDelta) / totalDelta;

  prevActualIdle_ = actualIdle;
  prevNonIdle_ = nonIdle;
}

float Processor::Utilization() {
  return currentUtilization_;
}