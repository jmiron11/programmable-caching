#ifndef STORAGE_MASTER_H_
#define STORAGE_MASTER_H_

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <thread>

#include "proto/storage_service.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class StorageMaster final : public MasterService::Service {
 public:
  // Thread safe data structure for mapping names to uri's
  class PeerTracker {
   public:
    std::string GetUri(const std::string& name);
    void AddPeer(const std::string& name, const std::string& uri);
  private:
    std::mutex tracker_mutex;
    std::unordered_map<std::string, std::string> name_to_uri_;
  };

  class StorageFileView {
   public:
  };


  StorageMaster(const std::string& hostname, const std::string& port);

  std::string GenerateName(const IntroduceRequest& reply);

  Status Introduce(ServerContext* context, const IntroduceRequest* request,
                   IntroduceReply* reply) override;

  Status Heartbeat(ServerContext* context, const Empty* request,
                   Empty* reply) override;

  Status InstallRule(ServerContext* context,
                     const InstallRuleRequest* request,
                     Empty* reply) override;

  Status RemoveRule(ServerContext* context,
                    const RemoveRuleRequest* request,
                    Empty* reply) override;

  void FillInRule(Rule* rule);

  void OffloadIntroduce(std::string name, std::string peer);

 private:
  // Maintains a mapping of the files to storage manager names.
  StorageFileView file_view_;

  // Maintains a view of the storage managers that are connected to the master.
  PeerTracker peer_tracker_;
};

#endif