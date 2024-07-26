#pragma once
#include <memory>
#include <thread>
#include <vector>

#include "client/rpc_client.h"
#include "monitor/cpu_load_monitor.h"
#include "monitor/cpu_softirq_monitor.h"
#include "monitor/cpu_stat_monitor.h"
#include "monitor/mem_monitor.h"
#include "monitor/monitor_inter.h"
#include "monitor/net_monitor.h"

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

int main() {
  //定义一个vector容器，用于存储指向MonitorInter接口的的智能指针
  std::vector<std::shared_ptr<monitor::MonitorInter>> runners_;

  //创建监控器对象并添加到runners_容器中，emplace_back与push_back相比，可以直接在容器的内存空间中构造元素，避免了额外的复制或移动操作
  runners_.emplace_back(new monitor::CpuSoftIrqMonitor());
  runners_.emplace_back(new monitor::CpuLoadMonitor());
  runners_.emplace_back(new monitor::CpuStatMonitor());
  runners_.emplace_back(new monitor::MemMonitor());
  runners_.emplace_back(new monitor::NetMonitor());
  
  //创建一个RpcClient对象，用于发送监控信息
  monitor::RpcClient rpc_client_;
  char *name = getenv("USER");

  //定义线程指针，初始化为nullptr
  std::unique_ptr<std::thread> thread_ = nullptr;
<<<<<<< HEAD

  //使用make_unique创建并初始化一个thread对象，并将结果赋值给thread，线程函数是一个lambda表达式，
  //[&]捕获了rpc_client_和runners_的引用，意味着在线程函数将共享这些变量
  thread_ = std::make_unique<std::thread>([&]() { 
=======
  thread_ = std::make_unique<std::thread>([&]() {
>>>>>>> e67a9ea38e015b6b5027e5b629c7a9762722496d
    while (true) {

      //创建一个MonitorInfo消息的实例，用于存储从监控器收集的数据
      monitor::proto::MonitorInfo monitor_info;
      monitor_info.set_name(std::string(name));

      //遍历每个监控器对象，对于每个监控器，调用UpdateOnce方法更新监测数据，并将数据放入monitor_info消息内
      for (auto& runner : runners_) {
        runner->UpdateOnce(&monitor_info);
      }

      //调用rpc_client对象发送填充好的消息，通常是通过网络发送到监控服务器
      rpc_client_.SetMonitorInfo(monitor_info);

      //当前线程休眠3秒
      std::this_thread::sleep_for(std::chrono::seconds(3));
    }
  });
  //调用join方法等待线程结束，会阻塞主线程，直到thread线程执行完毕
  thread_->join();
  return 0;
}
