#include "monitor/cpu_softirq_monitor.h"

#include "utils/read_file.h"
#include "utils/utils.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

//更新CPU软中断监测信息
namespace monitor {
void CpuSoftIrqMonitor::UpdateOnce(monitor::proto::MonitorInfo* monitor_info) {
  ReadFile softirqs_file(std::string("/proc/softirqs"));

  //由于/proc/softirqs中各个cpu软中断的排列方式与cpu_stat不同，因此通过二维数组获取信息
  std::vector<std::string> one_softirq;
  std::vector<std::vector<std::string>> softirq;

  //通过逐行读取将文件信息存放在二维数组内
  while (softirqs_file.ReadLine(&one_softirq)) {
    softirq.push_back(one_softirq);
    one_softirq.clear();
  }

  //逐个获取各个cpu的软中断信息
  for (int i = 0; i < softirq[0].size() - 1; i++) {
    std::string name = softirq[0][i];
    struct SoftIrq info;
    info.cpu_name = name;

    //stoll函数将字符串类型转为 long long型
    info.hi = std::stoll(softirq[1][i + 1]);
    info.timer = std::stoll(softirq[2][i + 1]);
    info.net_tx = std::stoll(softirq[3][i + 1]);
    info.net_rx = std::stoll(softirq[4][i + 1]);
    info.block = std::stoll(softirq[5][i + 1]);
    info.irq_poll = std::stoll(softirq[6][i + 1]);
    info.tasklet = std::stoll(softirq[7][i + 1]);
    info.sched = std::stoll(softirq[8][i + 1]);
    info.hrtimer = std::stoll(softirq[9][i + 1]);
    info.rcu = std::stoll(softirq[10][i + 1]);

    //使用boost chrono库中的steady_clock类获取现在的时间戳
    info.timepoint = boost::chrono::steady_clock::now();

    //获取此cpu上次存放的cpu中断信息
    auto iter = cpu_softirqs_.find(name);
    if (iter != cpu_softirqs_.end()) {
      struct SoftIrq& old = (*iter).second;

      //计算两个时间戳之间的时间间隔
      double period = Utils::SteadyTimeSecond(info.timepoint, old.timepoint);

      //使用add_soft_irq函数获取添加soft_irq列表的指针
      auto one_softirq_msg = monitor_info->add_soft_irq();
      one_softirq_msg->set_cpu(info.cpu_name);
      one_softirq_msg->set_hi((info.hi - old.hi) / period);
      one_softirq_msg->set_timer((info.timer - old.timer) / period);
      one_softirq_msg->set_net_tx((info.net_tx - old.net_tx) / period);
      one_softirq_msg->set_net_rx((info.net_rx - old.net_rx) / period);
      one_softirq_msg->set_block((info.block - old.block) / period);
      one_softirq_msg->set_irq_poll((info.irq_poll - old.irq_poll) / period);
      one_softirq_msg->set_tasklet((info.tasklet - old.tasklet) / period);
      one_softirq_msg->set_sched((info.sched - old.sched) / period);
      one_softirq_msg->set_hrtimer((info.hrtimer - old.hrtimer) / period);
      one_softirq_msg->set_rcu((info.rcu - old.rcu) / period);
    }
    //存放此次获取的cpu软中断信息，用于下次计算
    cpu_softirqs_[name] = info;
  }
  return;
}

}  // namespace monitor