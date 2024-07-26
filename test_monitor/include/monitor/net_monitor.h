#pragma once

#include <string>
#include <unordered_map>
#include <boost/chrono.hpp>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor {
class NetMonitor : public MonitorInter {
  struct NetInfo {
  // 网络接口的名称，例如 "eth0"、"wlan0" 等。
  std::string name;

  // 接收到的字节总数。
  int64_t rcv_bytes;

  // 接收到的数据包总数。
  int64_t rcv_packets;

  // 接口接收时因错误丢弃的数据包数。
  int64_t err_in;

  // 接口接收时因其他原因（如缓冲区溢出）丢弃的数据包数。
  int64_t drop_in;

  // 发送出去的字节总数。
  int64_t snd_bytes;

  // 发送出去的数据包总数。
  int64_t snd_packets;

  // 接口发送时因错误丢弃的数据包数。
  int64_t err_out;

  // 接口发送时因其他原因（如缓冲区溢出）丢弃的数据包数。
  int64_t drop_out;

  // 记录数据的时间点，使用 boost::chrono::steady_clock 以确保时间的稳定性。
  boost::chrono::steady_clock::time_point timepoint;
};

 public:
  NetMonitor() {}
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}

 private:
  std::unordered_map<std::string, struct NetInfo> net_info_;
};

}  // namespace monitor