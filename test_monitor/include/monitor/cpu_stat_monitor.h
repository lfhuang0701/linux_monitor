#pragma once

#include <string>
#include <unordered_map>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
class CpuStatMonitor : public MonitorInter {
  //此处的CpuStat与proto文件中定义的CpuStat不同，用命名空间区分
  struct CpuStat {
    std::string cpu_name;
    float user;                   //用户态cpu时间
    float system;                 //内核态cpu时间
    float idle;                   //空闲cpu时间
    float nice;                   //低优先级用户态cpu时间
    float io_wait;                //cpu等待I/O完成的时间
    float irq;                    //cpu处理硬件中断请求的时间
    float soft_irq;               //cpu处理软件中断的时间
    float steal;                  //cpu被其他虚拟cpu抢占的时间
    float guest;                  //cpu运行虚拟机中客户操作系统的时间
    float guest_nice;             //cpu以低优先级运行虚拟机中客户操作系统的时间
  };

 public:
  CpuStatMonitor() {}
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}

  //存放上一次各个cpu使用时间的stat信息
 private:
  std::unordered_map<std::string, struct CpuStat> cpu_stat_map_;
};

}  // namespace monitor
