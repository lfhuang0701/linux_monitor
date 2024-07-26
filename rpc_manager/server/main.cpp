#include <iostream>
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>
#include "rpc_manager.h"

//定义常量字符串，指定服务端监听的IP地址和端口号
constexpr char kServerPortInfo[] = "0.0.0.0:50051";

//初始化服务端函数
void InitServer() {

  //创建ServerBuilder对象，用于构建服务端
  grpc::ServerBuilder builder;

  //添加服务端监听端口
  builder.AddListeningPort(kServerPortInfo, grpc::InsecureServerCredentials());

  //创建服务实现对象
  monitor::GrpcManagerImpl grpc_server;

  //注册服务
  builder.RegisterService(&grpc_server);

  //构建并启动服务端
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  //等待服务端关闭，阻塞等待
  server->Wait();

  return;
}

int main() {
  InitServer();
  return 0;
}