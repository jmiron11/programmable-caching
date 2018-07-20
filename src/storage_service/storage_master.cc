
#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <thread>
#include <chrono>

#include "proto/storage_service.grpc.pb.h"
#include "storage_master.h"
#include "storage_manager_view.h"
#include "storage_file_view.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;

StorageMaster::StorageMaster(const std::string& hostname,
                             const std::string& port,
                             int master_disseminate_interval) {
  disseminate_thread_ = std::thread(&StorageMaster::DisseminateThread, this,
                                    master_disseminate_interval);

  std::string server_address(hostname + ':' + port);

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(this);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

std::string StorageMaster::GenerateName(StorageName name, StorageType type,
                                        const std::string& hostname) {
  return StorageName_Name(name) + StorageType_Name(type) + ":" + hostname;
}

Status StorageMaster::Introduce(ServerContext* context,
                                const IntroduceRequest* request,
                                IntroduceReply* reply) {
  std::string name = GenerateName(request->storage_name(),
                                  request->storage_type(), context->peer());
  std::thread offload(&StorageMaster::OffloadIntroduce, this, name,
                      request->rpc_hostname() + ":" + request->rpc_port());
  offload.detach();
  reply->set_name(name);
  return Status::OK;
}

void StorageMaster::OffloadIntroduce(std::string name, std::string peer) {
  manager_view_.Add(name, peer);
}

Status StorageMaster::Heartbeat(ServerContext* context,
                                const HeartbeatRequest* request,
                                HeartbeatReply* reply) {
  LOG(INFO) << "heartbeat received from " << context->peer();
  return Status::OK;
}

void StorageMaster::DisseminateThread(int master_disseminate_interval) {
  auto curr_t = std::chrono::system_clock::now();

  while (1) {
    auto next_t = curr_t + std::chrono::seconds{master_disseminate_interval};
    std::this_thread::sleep_until(next_t);
    curr_t = next_t;

    // TODO(justinmiron): Copies all over the place, this is awful.
    std::vector<StorageManagerService::Stub*> stubs = manager_view_.GetAllStubs();
    std::vector<std::pair<std::string, std::string>> data =
          manager_view_.GetAllInfo();

    DisseminateRequest request;
    for (auto& d : data)  {
      DisseminateRequest::ManagerDelta * delta = request.add_delta();
      delta->set_add_if_true(true);
      delta->set_name(d.first);
      delta->set_uri(d.second);
    }

    for (auto& stub : stubs) {
      DisseminateReply reply;
      ClientContext context;
      std::chrono::system_clock::time_point deadline =
        std::chrono::system_clock::now() +
        std::chrono::seconds(2);
      context.set_deadline(deadline);
      Status s = stub->DisseminateStorageManagers(&context, request, &reply);
      if (!s.ok()) {
        LOG(WARNING) << "Error disseminating to a storage manager";
      }
    }
  }
}
