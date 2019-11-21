//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#ifndef __TIMER_HPP
#define __TIMER_HPP

#include <chrono>

class timer {
private:
  typedef std::chrono::time_point<std::chrono::system_clock> time_t;

public:
  timer() : start_time(), stop_time() {}

  time_t start() { return (start_time = std::chrono::system_clock::now()); }
  time_t stop() { return (stop_time = std::chrono::system_clock::now()); }
  auto   elapsed() { return std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count(); }

private:
  time_t start_time, stop_time;
};

#endif    // __TIMER_HPP
