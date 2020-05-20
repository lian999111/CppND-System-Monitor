#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
//  #include <experimental/filesystem>  // use <experimental/filesystem> cuz gcc
//  v7 does not yet implement <filesystem>

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
  string dummy, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> dummy >> dummy >> kernel;
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
// Done
float LinuxParser::MemoryUtilization() {
  string line, key;
  float mem_total{1.0}, mem_free{0.0}, buffers{0.0};
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;

      if (key == "MemTotal:") {
        linestream >> mem_total;
      }

      if (key == "MemFree:") {
        linestream >> mem_free;
      }

      if (key == "Buffers:") {
        linestream >> buffers;
        break;
      }
    }
  }

  return 1.0 - (mem_free / (mem_total - buffers));
}

// TODO: Read and return the system uptime
// Done
long LinuxParser::UpTime() {
  long uptime{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    // Use ifstream directly
    stream >> uptime;
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system
// Done
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
// Done
long LinuxParser::ActiveJiffies() {
  vector<string> cpu_stat = CpuUtilization();
  long user = std::stol(cpu_stat[1]);
  long nice = std::stol(cpu_stat[2]);
  long system = std::stol(cpu_stat[3]);
  long irq = std::stol(cpu_stat[6]);
  long softirq = std::stol(cpu_stat[7]);
  long steal = std::stol(cpu_stat[8]);
  return user + nice + system + irq + softirq + steal;
}

// TODO: Read and return the number of idle jiffies for the system
// Done
long LinuxParser::IdleJiffies() {
  vector<string> cpu_stat = CpuUtilization();
  long idle = std::stol(cpu_stat[4]);
  long iowait = std::stol(cpu_stat[5]);
  return idle + iowait;
}

// TODO: Read and return CPU utilization
// Done
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_stat;  // the vector containing the values in the first
                            // line of /proc/stat
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    string line, value;
    // Get the first line
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        cpu_stat.emplace_back(value);
      }
    }
  }

  return cpu_stat;
}

// TODO: Read and return the total number of processes
// Done
int LinuxParser::TotalProcesses() {
  string line, key;
  int processes{0};
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> processes;
      }
    }
  }
  return processes;
}

// TODO: Read and return the number of running processes
// Done
int LinuxParser::RunningProcesses() {
  string line, key;
  int running_processes{0};
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> running_processes;
      }
    }
  }
  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }