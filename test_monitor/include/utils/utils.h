#pragma once

#include <boost/chrono.hpp>

//定义工具类 Utils，定义一个静态成员函数，可以在不实例化类的情况下调用；函数接收两个时间点，并计算两个时间点之间的时间间隔
//使用到了boost库的日期和时间处理功能，steady_clock是Boost Chrono库中定义的一个稳定时钟，提供了稳定的时间来源
namespace monitor {
class Utils {
 public:
  static double SteadyTimeSecond(
      const boost::chrono::steady_clock::time_point &t1,
      const boost::chrono::steady_clock::time_point &t2) {
    boost::chrono::duration<double> time_second = t1 - t2;
    return time_second.count();
  }
};
}  // namespace monitor