#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);                        // Ctor
  int Pid() const;                         // TODO: See src/process.cpp
  std::string User() const;                // TODO: See src/process.cpp
  std::string Command() const;             // TODO: See src/process.cpp
  void UpdateCpuUtilization();
  float CpuUtilization() const;            // TODO: See src/process.cpp
  std::string Ram() const;                 // TODO: See src/process.cpp
  long int UpTime() const;                 // TODO: See src/process.cpp
  bool operator>(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_{0};
  long start_time_{0};  // in jiffies
  long prev_total_{0};
  long prev_active_{0};
  float cpu_utilization_{0.0};
};

#endif