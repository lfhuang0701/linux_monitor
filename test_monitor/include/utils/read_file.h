//确保该头文件在其包含的编译单元中只被包含一次，避免重复包含
#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//定义读取文件的工具类，用于读取文件，并提供一些基本的文件操作功能
namespace monitor {
class ReadFile {
 public:
  //构造函数，使用explicit关键字告诉编译器只能使用该构造函数进行直接初始化
  //使用初始化列表进行初始化，ifs_是私有成员变量，其类型是ifstream类，用于从文件中读取数据，类比cin使用
  explicit ReadFile(const std::string& name) : ifs_(name) {}
  ~ReadFile() { ifs_.close(); }

  //读取文件的一行，并将该行分割为多个字符串，存储在args向量中，函数返回一个bool值，用于指示是否成功读取一行
  bool ReadLine(std::vector<std::string>* args);

  //读取指定文件的指定行数，并将每一行作为一个字符串存储在数组向量中，然后返回该数组向量
  static std::vector<std::string> GetStatsLines(const std::string& stat_file,
                                                const int line_count) {
    std::vector<std::string> stats_lines;
    std::ifstream buffer(stat_file);
    for (int line_num = 0; line_num < line_count; ++line_num) {
      std::string line;
      std::getline(buffer, line);
      if (line.empty()) {
        break;
      }
      stats_lines.push_back(line);
    }
    return stats_lines;
  }

 private:
  std::ifstream ifs_;
};
}  // namespace monitor
