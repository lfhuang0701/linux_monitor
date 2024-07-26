#include "rpc_client.h"

//实现rpc客户端类，通过grpc与远程服务器进行通信
namespace monitor {

//构造函数
RpcClient::RpcClient(const std::string& server_address) {

  //调用grpc::CreateChannel函数创建一个gRPC通道，用于与服务器在本地的50051端口上进行通信，并使用不安全凭据（grpc::InsecureChannelCredentials()），
  //这适用于测试和开发环境
  auto channel = grpc::CreateChannel(server_address,
                                     grpc::InsecureChannelCredentials());

  //使用NewStub方法创建一个gRPC存根，存根用于发起对服务端的远程调用，用智能指针指向该存根对象，其中
RpcClient::RpcClient(const std::string& server_address) {
  auto channel =
      grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
  stub_ptr_ = monitor::proto::GrpcManager::NewStub(channel);
  }
}
RpcClient::~RpcClient() {}


//将监控信息发送到gRPC服务端
void RpcClient::SetMonitorInfo(const monitor::proto::MonitorInfo& monito_info) {
  //创建一个gRPC客户端上下文对象，包含了RPC调用的元数据，如超时设置，取消信号
  ::grpc::ClientContext context; 

  //创建一个空的protobuf消息对象，用于接收响应
  ::google::protobuf::Empty response;

  //通过存根调用服务端的SetMonitorInfo方法，stub通过proto文件生成，封装了与服务器通信的细节
  ::grpc::Status status = stub_ptr_->SetMonitorInfo(&context, monito_info, &response);

  //检查调用是否成功
  if (status.ok()) {
  } else {
    std::cout << status.error_details() << std::endl;
    std::cout << "status.error_message: " << status.error_message()
              << std::endl;
    std::cout << "falied to connect !!!" << std::endl;
  }
}

//这个方法用于从gRPC服务端获取监控信息
void RpcClient::GetMonitorInfo(monitor::proto::MonitorInfo* monito_info) {
  ::grpc::ClientContext context;
  ::google::protobuf::Empty request;
  ::grpc::Status status =
      stub_ptr_->GetMonitorInfo(&context, request, monito_info);
  if (status.ok()) {
  } else {
    std::cout << status.error_details() << std::endl;
    std::cout << "status.error_message: " << status.error_message()
              << std::endl;
    std::cout << "falied to connect !!!" << std::endl;
  }
}
}  // namespace monitor
