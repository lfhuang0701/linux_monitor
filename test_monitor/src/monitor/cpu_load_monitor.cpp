#include "monitor/cpu_load_monitor.h"

#include "utils/read_file.h"

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

//更新cpu负载监测信息，传入参数是proto文件生成的监测信息类的实例，然后更新该实例
namespace monitor {
void CpuLoadMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {

  //实现读取文件类，读取"/proc/loadavg"文件获取平均负载信息
  ReadFile cpu_load_file(std::string("/proc/loadavg"));
  std::vector<std::string> cpu_load;

  //由于"/proc/loadavg"文件只有一行，因此读取该行数据
  cpu_load_file.ReadLine(&cpu_load);

  //使用stof函数将字符串转为float类型数据
  load_avg_1_ = std::stof(cpu_load[0]);
  load_avg_3_ = std::stof(cpu_load[1]);
  load_avg_15_ = std::stof(cpu_load[2]);

  //使用mutable_cpu_load函数获取指向cpu_load字段的指针，使用auto自动类型推导，cpu_load为自定义类型，因此使用mutable函数，而不能直接set
  auto cpu_load_msg = monitor_info->mutable_cpu_load();

  //使用set_XXX方法设置XXX字段的值
  cpu_load_msg->set_load_avg_1(load_avg_1_);
  cpu_load_msg->set_load_avg_3(load_avg_3_);
  cpu_load_msg->set_load_avg_15(load_avg_15_);

  return;
}

}  // namespace monitor