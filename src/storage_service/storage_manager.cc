#include <grpcpp/grpcpp.h>

#include "proto/storage_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class StorageManagerClient {
 public:
  StorageManagerClient(std::shared_ptr<Channel> channel)
      : stub_(Master::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int SayHello() {
    // Data we are sending to the server.
    IntroduceRequest request;
    request.set_storage_type(StorageType::EPHEMERAL);

    // Container for the data we expect from the server.
    IntroduceReply reply;
    ClientContext context;
    Status status = stub_->Introduce(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      std::cout << reply.name() << std::endl;
      return 0;
    }
    else{
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }

 private:
  std::unique_ptr<Master::Stub> stub_;
};

int main(int argc, char** argv) {
  StorageManagerClient manager(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  int reply = manager.SayHello();
  return 0;
}