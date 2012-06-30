#include <sys/time.h>
#include <iostream>

struct __bench__
{
  double start;
  __bench__() {
    start = sec();
  }
  ~__bench__() {
    std::cout << sec() - start << std::endl;
  }
  double sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec* 1e-6;
  }
  operator bool() {
    return false;
  }
};


#define benchmark if (__bench__ __b__ = __bench__()) {} else
