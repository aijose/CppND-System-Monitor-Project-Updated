#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

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
  remap  gt nnoremap if : tabedit nnoremap <ScrollWheelDown>
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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  long mem_total=0.0, mem_free=0.0;
  long value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal") {
        mem_total = value;
      }
      else if (key == "MemFree") {
        mem_free = value;
      }
    }
  }
  return (mem_total - mem_free)*1.0/mem_total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string key;
  float uptime=0.0, idletime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> uptime >> idletime;
      return static_cast<long>(uptime);
    }
  }
  return static_cast<long>(uptime);
}

// TODO: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
//long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
//long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<unsigned long long int> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  unsigned long long int single_entry;
  vector<unsigned long long int> output;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
          std::istringstream linestream(line);
          linestream >> key;
          if (key == "cpu") {
              for(int i=0; i < 10; i++) {
                  linestream >> single_entry;
                  output.push_back(single_entry);
              }
              return output;
          }
      }
  }
  return output;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value=0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes")
          return value;
    }
  }
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value=0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running")
          return value;
    }
  }
  return value;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string file = kProcDirectory + to_string(pid) + kCmdlineFilename;
  string cmd="";
  string line;
  std::ifstream filestream(file);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    for(auto it=line.begin(); it != line.end(); it++)
        if(iscntrl(*it)) (*it) = ' ';
    std::istringstream linestream(line);
    linestream >> cmd;
    // Add trailing spaces that will overwrite pre-existing 
    // characters in ncurses
    cmd = cmd + string(80, ' ');
    return cmd;
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value="None";
  string file = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream filestream(file);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:")
          return value;
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value="None";
  string file = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream filestream(file);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:")
          return value;
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string username="";
  string value1="None";
  string value2="None";
  string uid = LinuxParser::Uid(pid);
  string file = kPasswordPath;
  std::ifstream filestream(file);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> value1 >> value2) {
          if (value2 == uid)
              return username;
          username="";
      }
    }
  }
  return username;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
    string line;
    string entry;
    long starttime = 0;
    string filename = kProcDirectory + to_string(pid) + kStatFilename;
    std::ifstream filestream(filename);
    if (filestream.is_open()) {
        for(int i=0; i < 22; i++)
            filestream >> entry;
        starttime = std::stoi(entry);
    }
    float system_uptime = static_cast<float>(LinuxParser::UpTime());
    float seconds = system_uptime - starttime/sysconf(_SC_CLK_TCK);

    return seconds;
}

// TODO: Read and return the uptime of a process
float LinuxParser::CpuUtilization(int pid) { 
    float total_time, seconds, cpu_usage, uptime;
    long utime, stime, cutime, cstime, starttime;
    string line;
    string string_entry;
    long long_entry;
    string filename = kProcDirectory + to_string(pid) + kStatFilename;
    std::ifstream filestream(filename);
    if (filestream.is_open()) {
        for(int i=0; i < 3; i++)
            filestream >> string_entry;

        int count = 3;
        while(filestream >> long_entry) {
            count++;
            switch(count) {
                case 14:
                    utime = long_entry;
                    break;
                case 15:
                    stime = long_entry;
                    break;
                case 16:
                    cutime = long_entry;
                    break;
                case 17:
                    cstime = long_entry;
                    break;
                case 22:
                    starttime = long_entry;
                    break;
                default:
                    break;
            }
        }
    }
    uptime = static_cast<float>(LinuxParser::UpTime());
    total_time = utime + stime;
    total_time += cutime + cstime; 
    seconds = uptime - starttime/sysconf(_SC_CLK_TCK);
    cpu_usage = (total_time/sysconf(_SC_CLK_TCK) / seconds);
    return cpu_usage;
}
