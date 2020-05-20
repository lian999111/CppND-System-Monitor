#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long prevTotal_{0};   // previous total jiffies
  long prevActive_{0};  // previous active jiffies
};

#endif