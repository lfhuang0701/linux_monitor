#pragma once

#include <string>
#include <unordered_map>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

//与cpu_load和cpu_softirqs类似
namespace monitor {
class MemMonitor : public MonitorInter {
  struct MenInfo {
    // 总内存量（包括缓冲区/缓存）
    int64_t total;

    // 空闲内存量（未被使用的内存）
    int64_t free;

    // 可用内存量（可立即用于分配的内存，考虑了磁盘交换的影响）
    int64_t avail;

    // 用作缓冲的内存量
    int64_t buffers;

    // 缓存的内存量
    int64_t cached;

    // 交换缓存的内存量（曾经被写入磁盘，现在再次被使用）
    int64_t swap_cached;

    // 活跃使用的内存量（最近被使用，不太可能被回收）
    int64_t active;

    // 不活跃使用的内存量（最近未使用，可能被回收）
    int64_t in_active;

    // 活跃匿名内存量（不被文件备份的活跃内存）
    int64_t active_anon;

    // 不活跃匿名内存量（不被文件备份的不活跃内存）
    int64_t inactive_anon;

    // 活跃文件内存量（被文件备份的活跃内存）
    int64_t active_file;

    // 不活跃文件内存量（被文件备份的不活跃内存）
    int64_t inactive_file;

    // 脏页内存量（需要写回到磁盘的内存）
    int64_t dirty;

    // 正在被写回的内存量
    int64_t writeback;

    // 匿名页内存量（被匿名进程使用的内存）
    int64_t anon_pages;

    // 映射到文件的内存量（包括共享库等）
    int64_t mapped;

    // 可回收的内核内存量（Slab 内存，可以回收）
    int64_t kReclaimable;

    // 可回收的文件内存量（文件缓存，可以回收）
    int64_t sReclaimable;

    // 不可回收的文件内存量（文件缓存，不能回收）
    int64_t sUnreclaim;
  };

 public:
  MemMonitor() {}
  void UpdateOnce(monitor::proto::MonitorInfo* monitor_info);
  void Stop() override {}
};

}  // namespace monitor