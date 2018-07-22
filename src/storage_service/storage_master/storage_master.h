#ifndef STORAGE_MASTER_H_
#define STORAGE_MASTER_H_

#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <thread>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class StorageMaster final : public MasterService::Service {
 public:
  // Thread safe data structure for mapping names to uri's
  class PeerTracker {
   public:
    class Peer {
     public:
      Peer(const std::string& _name, const std::string& _rpc_uri,
                 const std::string& _connection_uri):
        name(_name), rpc_uri(_rpc_uri), connection_uri(_connection_uri){ }
      std::string name;
      std::string rpc_uri;
      std::string connection_uri;
    };
    Peer GetPeerFromName(const std::string& name) const;
    Peer GetPeerFromConnection(const std::string& name) const;
    void AddPeer(const std::string& name, const std::string& rpc_uri,
                 const std::string& connection_uri);
   private:
    mutable std::mutex tracker_mutex;
    std::unordered_map<std::string, std::shared_ptr<Peer>> name_to_peer_;
    std::unordered_map<std::string, std::shared_ptr<Peer>> connection_to_peer_;
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
    mutable std::mutex view_mutex;
    std::unordered_map<std::string, ManagerFileView> manager_views;
  };

  StorageMaster(const std::string& hostname, const std::string& port);

  void Start(); // Start the storage master service
  void Stop(); // Stops the storage master service

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
  std::unique_ptr<Server> server_;
};

#endif