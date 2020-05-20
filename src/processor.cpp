#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long currTotal = LinuxParser::Jiffies();
  long currActive = LinuxParser::ActiveJiffies();
  float utilization =
      static_cast<float>(currActive - prevActive_) / (currTotal - prevTotal_);
  prevTotal_ = currTotal;
  prevActive_ = currActive;
  return utilization;
}