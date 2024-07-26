#pragma once

#include <string>
#include <unordered_map>

#include <boost/chrono.hpp>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
class CpuSoftIrqMonitor : public MonitorInter {
  struct SoftIrq {
  // 处理软件中断的 CPU 名称或编号。
  std::string cpu_name;

  // Hi 软件中断的处理次数。
  int64_t hi;

  // Timer 软件中断的处理次数，通常与周期性事件相关。
  int64_t timer;

  // 网络传输（net_tx）软件中断的处理次数。
  int64_t net_tx;

  // 网络接收（net_rx）软件中断的处理次数。
  int64_t net_rx;

  // Block I/O 请求的软件中断处理次数。
  int64_t block;

  // IRQ poll 软件中断的处理次数，用于不支持硬件中断的设备。
  int64_t irq_poll;

  // Tasklet 软件中断的处理次数，用于处理延迟执行的任务。
  int64_t tasklet;

  // 调度器（scheduler）软件中断的处理次数。
  int64_t sched;

  // 高分辨率定时器（hrtimer）软件中断的处理次数。
  int64_t hrtimer;

  // RCU（Read-Copy-Update）软件中断的处理次数，用于优化多处理器上的内存更新。
  int64_t rcu;

  // 记录数据的时间点，使用 boost::chrono::steady_clock 以确保时间的稳定性。
  boost::chrono::steady_clock::time_point timepoint;
};

 public:
  CpuSoftIrqMonitor() {}
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}

  //存放上次获取的各个cpu的中断信息
 private:
  std::unordered_map<std::string, struct SoftIrq> cpu_softirqs_;
};

}  // namespace monitor
