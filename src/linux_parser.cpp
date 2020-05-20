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
// Done
long LinuxParser::ActiveJiffies(int pid) {
  long utime{0}, stime{0}, cutime{0}, cstime{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    string value;
    for (int idx = 1; idx < 18; ++idx) {
      stream >> value;
      switch (idx) {
        case 14:
          utime = std::stol(value);
          break;

        case 15:
          stime = std::stol(value);
          break;

        case 16:
          cutime = std::stol(value);
          break;

        case 17:
          cstime = std::stol(value);
          break;
      }
    }
  }
  return utime + stime + cutime + cstime;
}

// TODO: Read and return the number of active jiffies for the system
// Done
long LinuxParser::ActiveJiffies() {
  vector<string> cpu_stat = CpuUtilization();
  long user = std::stol(cpu_stat[CPUStates::kUser_]);
  long nice = std::stol(cpu_stat[CPUStates::kNice_]);
  long system = std::stol(cpu_stat[CPUStates::kSystem_]);
  long irq = std::stol(cpu_stat[CPUStates::kIRQ_]);
  long softirq = std::stol(cpu_stat[CPUStates::kSoftIRQ_]);
  long steal = std::stol(cpu_stat[CPUStates::kSteal_]);
  return user + nice + system + irq + softirq + steal;
}

// TODO: Read and return the number of idle jiffies for the system
// Done
long LinuxParser::IdleJiffies() {
  vector<string> cpu_stat = CpuUtilization();
  long idle = std::stol(cpu_stat[CPUStates::kIdle_]);
  long iowait = std::stol(cpu_stat[CPUStates::kIOwait_]);
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
    // Get only the first line
    if (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        cpu_stat.push_back(value);
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
        break;
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
        break;
      }
    }
  }
  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// Done
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// Done
string LinuxParser::Uid(int pid) {
  string line, key, uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// Done
string LinuxParser::User(int pid) {
  string line, dummy, curr_uid, user;
  string uid{Uid(pid)};
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> dummy >> curr_uid;
      if (uid == curr_uid) {
        break;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
// Done
long LinuxParser::UpTime(int pid) {
  long sys_uptime = UpTime();  // in seconds
  long process_uptime = sys_uptime - StartTime(pid) / sysconf(_SC_CLK_TCK);
  return process_uptime;
}

long LinuxParser::StartTime(int pid) {
  long start_time{0};
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    string value;
    for (int idx = 1; idx < 23; ++idx) {
      stream >> value;
      // Get the 22nd value
      if (idx == 22) {
        start_time = std::stol(value);
      }
    }
  }
  return start_time;  // in jiffies
}