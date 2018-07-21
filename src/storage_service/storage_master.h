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

  // Thread safe data structure for managing a global view of the storage
  // service.
  class StorageFileView {
   public:
    class ManagerFileView {
     public:
      void AddFile(const std::string& file_key);
      void RemoveFile(const std::string& file_key);
      void PopulateManagerViewReply(GetViewReply::ManagerView* view_reply) const;
     private:
      std::unordered_map <std::string, int> file_keys;
    };

    void AddFileToManager(const std::string& manager_name,
                          const std::string& file_key);
    void RemoveFileFromManager(const std::string& manager_name,
                               const std::string& file_key);
    void PopulateViewReply(GetViewReply * reply) const;

   private:
    std::mutex view_mutex;
    std::unordered_map<std::string, ManagerFileView> manager_views;
  };

  StorageMaster(const std::string& hostname, const std::string& port);

  void Start(); // Start the storage master service
  void Stop(); // Stops the storage master service
  void MasterThread();

  std::string GenerateName(const IntroduceRequest& reply);

  Status Introduce(ServerContext* context, const IntroduceRequest* request,
                   IntroduceReply* reply) override;

  Status Heartbeat(ServerContext* context, const Empty* request,
                   Empty* reply) override;

  Status StorageChange(ServerContext* context,
                       const StorageChangeRequest* request,
                       Empty* reply);

  Status InstallRule(ServerContext* context,
                     const InstallRuleRequest* request,
                     Empty* reply) override;

  Status RemoveRule(ServerContext* context,
                    const RemoveRuleRequest* request,
                    Empty* reply) override;

  Status GetView(ServerContext* context,
                 const Empty* request,
                 GetViewReply* reply) override;


  void FillInRule(Rule* rule);

  void OffloadIntroduce(std::string name, std::string peer);

 private:
  // Maintains a mapping of the files to storage manager names.
  StorageFileView file_view_;

  // Maintains a view of the storage managers that are connected to the master.
  PeerTracker peer_tracker_;


  std::string master_hostname_;
  std::string master_port_;

  std::thread master_thread_;
  std::unique_ptr<Server> server_;
};

#endif