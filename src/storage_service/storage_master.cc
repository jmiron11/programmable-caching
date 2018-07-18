
#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

#include "proto/storage_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class MasterServiceImpl final : public MasterService::Service {
  Status Introduce(ServerContext* context, const IntroduceRequest* request,
                   IntroduceReply* reply) override {
    LOG(INFO) << "introduction received";
    return Status::OK;
  }

  Status Heartbeat(ServerContext* context, const HeartbeatRequest* request,
                   HeartbeatReply* reply) override {
    LOG(INFO) << "heartbeat received from " << context->peer();
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  MasterServiceImpl service;

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
