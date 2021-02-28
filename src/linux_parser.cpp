#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal = 1.0;
  float memFree = 1.0;

  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // remove extra whitespace
      line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

      std::replace(line.begin(), line.end(), ':', ' ');
      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          memTotal = stof(value.substr(0, line.size() - 2)); // convert value to float after removing kb from the end of the string
        }
        else if (key == "MemFree") {
          memFree = stof(value.substr(0, line.size() - 2)); // convert value to float after removing kb from the end of the string
        }
      }
    }
  }
  return (memTotal - memFree) / memTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stof(uptime); 
}


// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string cpu;
  vector<string> stats(10, "");
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> cpu >> stats[kUser_] >> stats[kNice_] >> stats[kSystem_] >> stats[kIdle_] >> stats[kIOwait_] >> stats[kIRQ_] >> stats[kSoftIRQ_] >> stats[kSteal_] >> stats[kGuest_] >> stats[kGuestNice_]) {
        if (cpu == "cpu") {
          return stats;
        }
      }
    }
  }

  return stats;
}

int LinuxParser::GetIntFromFile(string path, string key) {
  string line;
  string k;
  string value;
  std::ifstream filestream(path);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {      
      std::istringstream linestream(line);
      while (linestream >> k >> value) {
        if (k == key) {
          return stoi(value);
        }
      }
    }
  }
  
  return 0;
}

int LinuxParser::TotalProcesses() {
  return GetIntFromFile(kProcDirectory + kStatFilename, "processes");
}

int LinuxParser::RunningProcesses() { 
  return GetIntFromFile(kProcDirectory + kStatFilename, "procs_running");
}

string LinuxParser::GetProcessStat(string statname, int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == statname) {
          return value;
        }
      }
    }
  }
  
  return "";
}

float LinuxParser::CpuUtilization(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  float utime, stime, cutime, cstime, starttime = 0.0;
  
  if (stream.is_open()) {
   while (std::getline(stream, line)) {
     std::istringstream linestream(line);
     int count = 0;
     string value;
     while (linestream >> value) {
       count++;
       if (count == kCpuUtime) {
         utime = stof(value);
       }
       else if (count == kCpuStime) {
         stime = stof(value);
       }
       else if (count == kCpuCutime) {
         cutime = stof(value);
       }
       else if (count == kCpuCstime) {
         cstime = stof(value);
       }
       else if (count == kCpuStarttime) {
         starttime = stof(value);
       }
     }
    }
  }
  
  float total_time = utime + stime + cutime + cstime;
  float seconds = UpTime() - (starttime / sysconf(_SC_CLK_TCK));
  return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      return line;
    }
  }
  
  return "";
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  long ram = 0;
  string ramString = GetProcessStat("VmSize", pid);
  
  if (ramString != "") {
	  ram = stol(ramString);
  }
  
  return to_string(ram/1000);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return GetProcessStat("Uid", pid);
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, name, x, id;
  
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
   while (std::getline(stream, line)) {
     std::replace(line.begin(), line.end(), ':', ' ');
     std::istringstream linestream(line);
     while (linestream >> name >> x >> id) {
       if (id == uid) {
         return name;
       }
     }     
   }
  }
  
  return "UNKNOWN_USER";
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (stream.is_open()) {
   while (std::getline(stream, line)) {
     std::istringstream linestream(line);
     int count = 0;
     string value;
     while (linestream >> value) {
       count++;
       if (count == kUpTimePosition) {
         long ticks = stol(value);
         return ticks / sysconf(_SC_CLK_TCK);
       }
     }
   }
  }
  
  return 0;
}
