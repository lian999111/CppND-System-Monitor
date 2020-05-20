#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
// Done
vector<Process>& System::Processes() {
  // Read pids
  vector<int> pids{LinuxParser::Pids()};

  set<int> present_pids;
  for (const auto& process : processes_) {
    present_pids.insert(process.Pid());
  }

  // Add new processes
  for (const auto pid : pids) {
    if (present_pids.find(pid) == present_pids.end()) {
      processes_.emplace_back(pid);
    }
  }

  // Update cpu utilizations
  for (auto& process : processes_) {
    process.UpdateCpuUtilization();
  }

  // Sort processes based on cpu utilizations
  std::sort(processes_.begin(), processes_.end(), std::greater<Process>());

  return processes_;
}

// TODO: Return the system's kernel identifier (string)
// Done
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
// Done
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
// Done
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
// Done
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
// Done
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
// Done
long int System::UpTime() { return LinuxParser::UpTime(); }