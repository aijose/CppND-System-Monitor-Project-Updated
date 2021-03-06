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

Process::Process(int pid) {
    pid_ = pid;
    uid_ = LinuxParser::User(pid);
    cmd_ = LinuxParser::Command(pid);
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() const { 
    return LinuxParser::CpuUtilization(pid_); 
}

string Process::Command() { return cmd_; }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return uid_; }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const { 
    return CpuUtilization() > a.CpuUtilization();
}
