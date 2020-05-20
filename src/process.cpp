#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  start_time_ = LinuxParser::StartTime(pid);
}

// TODO: Return this process's ID
// Done
int Process::Pid() const { return pid_; }

void Process::UpdateCpuUtilization() {
  long curr_total = LinuxParser::Jiffies() - start_time_;
  long curr_active = LinuxParser::ActiveJiffies(pid_);
  cpu_utilization_ = static_cast<float>(curr_active - prev_active_) / (curr_total - prev_total_);
  prev_total_ = curr_total;
  prev_active_ = curr_active;
}

// TODO: Return this process's CPU utilization
// Done
float Process::CpuUtilization() const {
  return cpu_utilization_;
}

// TODO: Return the command that generated this process
// Done
string Process::Command() const { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return string(); }

// TODO: Return the user (name) that generated this process
// Done
string Process::User() const { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
// Done
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// Done
bool Process::operator>(Process const& a) const {
  return this->cpu_utilization_ > a.cpu_utilization_;
}