#include "rpc_manager.h"
#include <iostream>

//gRPC服务端，主要实现了两个RPC方法，
namespace monitor {
GrpcManagerImpl::GrpcManagerImpl() {}

GrpcManagerImpl::~GrpcManagerImpl() {}

//实现了SetMonitorInfo RPC方法，该方法接收客户端发送的监控信息，并返回一个空响应
::grpc::Status GrpcManagerImpl::SetMonitorInfo(
    ::grpc::ServerContext* context,
    const ::monitor::proto::MonitorInfo* request,
    ::google::protobuf::Empty* response) {

  //清除当前存储的监控信息
  monitor_infos_.Clear();

  //将客户端发来的请求中的监控信息复制到服务端的存储中
  monitor_infos_ = *request;
  std::cout << "jinru" << request->soft_irq_size() << std::endl;
  return grpc::Status::OK;
}

//实现了GetMonitorInfo RPC方法，该方法返回当前存储的监控信息给客户端
::grpc::Status GrpcManagerImpl::GetMonitorInfo(
    ::grpc::ServerContext* context, const ::google::protobuf::Empty* request,
    ::monitor::proto::MonitorInfo* response) {
  *response = monitor_infos_;
  return grpc::Status::OK;
}

}  // namespace monitor
