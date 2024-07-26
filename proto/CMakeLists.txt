
# 此cmake脚本的作用是构建proto项目，使其编译proto文件定义的数据结构和grpc服务，生成相应的c++代码，并将其链接到一个库中

# 查找并配置protobuf库，CONFIG表示使用protobuf-config.cmake文件，REQUIRED选项代表如果找不到，cmake将报错并停止
find_package(protobuf CONFIG REQUIRED)

# 查找并配置grpc库
find_package(gRPC CONFIG REQUIRED)

# 查找并配置c-ares库，这是一个异步DNS解析库，常用于grpc以提高性能
find_package(c-ares CONFIG)
# find_package(Threads)

#
# Protobuf/Grpc source files，设置PROTO_FILES变量，使其包含所有需要编译的.proto文件列表
#
set(PROTO_FILES
    monitor_info.proto
    cpu_load.proto
    cpu_softirq.proto
    cpu_stat.proto
    mem_info.proto
    net_info.proto
)

#
# Add Library target with protobuf sources
#
# 添加库目标 monitor_proto，它由前面设置的PROTO_FILES变量中的.proto文件组成
add_library(monitor_proto ${PROTO_FILES})

#将protobuf库和grpc库链接到monitor_proto库目标
target_link_libraries(monitor_proto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
# 为monitor_proto指定包含目录，以便编译器能够找到protobuf的头文件以及生成的头文件
target_include_directories(monitor_proto PUBLIC
${PROTOBUF_INCLUDE_DIRS} 
${CMAKE_CURRENT_BINARY_DIR})

#
# Compile protobuf and grpc files in mointor proto target to cpp
#
# 获取grpc C++插件的位置，之后用于生成grpc相关的C++代码
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

# 使用protobuf生成工具编译proto文件生成'.pb.h'和'.pb.cc'文件，这些文件包含关于proto文件中定义的数据结构的使用方法
protobuf_generate(TARGET monitor_proto LANGUAGE cpp)

# 使用protobuf生成工具编译proto文件，并使用grpc插件生成带有‘.grpc.pb.h’和'.grpcpb.cc'扩展名的文件，这些文件包含grpc服务的实现
protobuf_generate(TARGET monitor_proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
