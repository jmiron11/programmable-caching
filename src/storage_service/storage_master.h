#ifndef STORAGE_MASTER_H_
#define STORAGE_MASTER_H_

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <thread>

#include "proto/storage_service.grpc.pb.h"
#include "storage_manager_view.h"
#include "storage_file_view.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class StorageMaster final : public MasterService::Service {
 public:
  StorageMaster(const std::string& hostname, const std::string& port,
                int master_disseminate_interval_);

  std::string GenerateName(StorageName name, StorageType type,
                           const std::string& hostname);

  Status Introduce(ServerContext* context, const IntroduceRequest* request,
                   IntroduceReply* reply) override;

  Status Heartbeat(ServerContext* context, const HeartbeatRequest* request,
                   HeartbeatReply* reply) override;

  void OffloadIntroduce(std::string name, std::string peer);

  void DisseminateThread(int master_disseminate_interval);

 private:
  // Maintains a mapping of the files to storage manager names.
  StorageFileView file_view_;

  // Maintains a view of the storage managers that are connected to the master.
  StorageManagerView manager_view_;

  // Thread that disseminates the view of the storage managers on a regular interview.
  std::thread disseminate_thread_;
};

#endif