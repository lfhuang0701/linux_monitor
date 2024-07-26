#include "monitor/cpu_stat_monitor.h"

#include "utils/read_file.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"


namespace monitor {
void CpuStatMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
  ReadFile cpu_stat_file(std::string("/proc/stat"));
  std::vector<std::string> cpu_stat_list;
  while (cpu_stat_file.ReadLine(&cpu_stat_list)) {
    if (cpu_stat_list[0].find("cpu") != std::string::npos) {
      // std::cout << cpu_stat_list[0] << cpu_stat_list[1] << std::endl;
      struct CpuStat cpu_stat;
      cpu_stat.cpu_name = cpu_stat_list[0];
      cpu_stat.user = std::stof(cpu_stat_list[1]);
      cpu_stat.nice = std::stof(cpu_stat_list[2]);
      cpu_stat.system = std::stof(cpu_stat_list[3]);
      cpu_stat.idle = std::stof(cpu_stat_list[4]);
      cpu_stat.io_wait = std::stof(cpu_stat_list[5]);
      cpu_stat.irq = std::stof(cpu_stat_list[6]);
      cpu_stat.soft_irq = std::stof(cpu_stat_list[7]);
      cpu_stat.steal = std::stof(cpu_stat_list[8]);
      cpu_stat.guest = std::stof(cpu_stat_list[9]);
      cpu_stat.guest_nice = std::stof(cpu_stat_list[10]);

      //获取上次的cpu_stat信息
      auto it = cpu_stat_map_.find(cpu_stat.cpu_name);
      if (it != cpu_stat_map_.end()) {
        // std::cout << cpu_stat.cpu_name << std::endl;
        struct CpuStat old = it->second;

        //add_cpu_stat函数是MonitorInfo类的一个成员函数，用于在MonitiorInfo消息的cpu_stat列表中添加一个新的CpuStat消息
        //proto文件中对cpu_stat使用了repeted关键字，说明cpu_stat是一个列表而非字段，也就是类似数组的意思（因为一共有四个cpu），因此这里使用添加，
        //这里有个问题，cpu_stat列表会一直添加吗，数组大小正常来说是4，这里一直添加合理吗。
        auto cpu_stat_msg = monitor_info->add_cpu_stat();

        //总时间 = 用户态cpu时间 + 内核态cpu时间 + cpu空闲时间 + cpu等待io时间 + cpu处理硬中断时间 + cpu处理软中断时间 + 被其他虚拟cpu抢占的时间
        //忽略了运行虚拟机的时间
        float new_cpu_total_time = cpu_stat.user + cpu_stat.system +
                                   cpu_stat.idle + cpu_stat.nice +
                                   cpu_stat.io_wait + cpu_stat.irq +
                                   cpu_stat.soft_irq + cpu_stat.steal;
        float old_cpu_total_time = old.user + old.system + old.idle + old.nice +
                                   old.io_wait + old.irq + old.soft_irq +
                                   old.steal;
        float new_cpu_busy_time = cpu_stat.user + cpu_stat.system +
                                  cpu_stat.nice + cpu_stat.irq +
                                  cpu_stat.soft_irq + cpu_stat.steal;
        float old_cpu_busy_time = old.user + old.system + old.nice + old.irq +
                                  old.soft_irq + old.steal;

        //计算各部分的cpu使用时间占比，即cpu使用率，计算公式是 使用时间/空闲时间
        float cpu_percent = (new_cpu_busy_time - old_cpu_busy_time) /
                            (new_cpu_total_time - old_cpu_total_time) * 100.00;
        float cpu_user_percent = (cpu_stat.user - old.user) /
                                 (new_cpu_total_time - old_cpu_total_time) *
                                 100.00;
        float cpu_system_percent = (cpu_stat.system - old.system) /
                                   (new_cpu_total_time - old_cpu_total_time) *
                                   100.00;
        float cpu_nice_percent = (cpu_stat.nice - old.nice) /
                                 (new_cpu_total_time - old_cpu_total_time) *
                                 100.00;
        float cpu_idle_percent = (cpu_stat.idle - old.idle) /
                                 (new_cpu_total_time - old_cpu_total_time) *
                                 100.00;
        float cpu_io_wait_percent = (cpu_stat.io_wait - old.io_wait) /
                                    (new_cpu_total_time - old_cpu_total_time) *
                                    100.00;
        float cpu_irq_percent = (cpu_stat.irq - old.irq) /
                                (new_cpu_total_time - old_cpu_total_time) *
                                100.00;
        float cpu_soft_irq_percent = (cpu_stat.soft_irq - old.soft_irq) /
                                     (new_cpu_total_time - old_cpu_total_time) *
                                     100.00;
        //添加cpu_stat信息
        cpu_stat_msg->set_cpu_name(cpu_stat.cpu_name);
        cpu_stat_msg->set_cpu_percent(cpu_percent);
        cpu_stat_msg->set_usr_percent(cpu_user_percent);
        cpu_stat_msg->set_system_percent(cpu_system_percent);
        cpu_stat_msg->set_nice_percent(cpu_nice_percent);
        cpu_stat_msg->set_idle_percent(cpu_idle_percent);
        cpu_stat_msg->set_io_wait_percent(cpu_io_wait_percent);
        cpu_stat_msg->set_irq_percent(cpu_irq_percent);
        cpu_stat_msg->set_soft_irq_percent(cpu_soft_irq_percent);
      }
      //保存此次获取的cpu使用时间信息，
      cpu_stat_map_[cpu_stat.cpu_name] = cpu_stat;
    }
    //清空数组，用于下个cpu
    cpu_stat_list.clear();
  }

  return;
}

}  // namespace monitor