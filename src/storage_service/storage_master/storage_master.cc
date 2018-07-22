#include "proto/storage_service.grpc.pb.h"
#include "storage_master.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <thread>
#include <chrono>
#include <memory>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;

StorageMaster::PeerTracker::Peer StorageMaster::PeerTracker::GetPeerFromName(
  const std::string& name) const {
  // TODO(justinmiron): Asking for a segfault.
  return *name_to_peer_.at(name);
}

StorageMaster::PeerTracker::Peer
StorageMaster::PeerTracker::GetPeerFromConnection(const std::string& conn)
const {
  // TODO(justinmiron): Asking for a segfault.
  return *connection_to_peer_.at(conn);
}

void StorageMaster::PeerTracker::AddPeer(const std::string& name,
    const std::string& rpc_uri, const std::string& connection_uri) {
  std::lock_guard<std::mutex> lock(tracker_mutex);
  name_to_peer_[name] = std::make_shared<Peer>(name, rpc_uri, connection_uri);
  connection_to_peer_[connection_uri] = name_to_peer_[name];
}

void StorageMaster::StorageFileView::ManagerFileView::AddFile(
  const std::string& file_key) {
  file_keys[file_key] = 0;
}

void StorageMaster::StorageFileView::ManagerFileView::RemoveFile(
  const std::string& file_key) {
  file_keys.erase(file_key);
}

void StorageMaster::StorageFileView::ManagerFileView::PopulateManagerViewReply(
  GetViewReply::ManagerView* view_reply) const {
  for (const auto& files : file_keys) {
    view_reply->add_key(files.first);
  }
}

void StorageMaster::StorageFileView::AddFileToManager(const std::string&
    manager_name,
    const std::string& file_key) {
  std::lock_guard<std::mutex> lock(view_mutex);
  StorageMaster::StorageFileView::ManagerFileView& view =
    manager_views[manager_name];
  view.AddFile(file_key);
}

void StorageMaster::StorageFileView::RemoveFileFromManager(
  const std::string& manager_name,
  const std::string& file_key) {
  std::lock_guard<std::mutex> lock(view_mutex);
  StorageMaster::StorageFileView::ManagerFileView& view =
    manager_views[manager_name];
  view.RemoveFile(file_key);
}

void StorageMaster::StorageFileView::PopulateViewReply(GetViewReply * reply)
const {
  std::lock_guard<std::mutex> lock(view_mutex);
  for (const auto& name_view_pair : manager_views) {
    const std::string& name = name_view_pair.first;
    const StorageMaster::StorageFileView::ManagerFileView& view =
      name_view_pair.second;

    GetViewReply::ManagerView* msg_manager_view = reply->add_view();
    msg_manager_view->set_name(name);
    view.PopulateManagerViewReply(msg_manager_view);
  }
}

StorageMaster::StorageMaster(const std::string& hostname,
                             const std::string& port):
  master_hostname_(hostname), master_port_(port) {}

void StorageMaster::Start() {
  std::string server_address(master_hostname_ + ':' + master_port_);
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(this);
  server_ = builder.BuildAndStart();
  LOG(INFO) << "Storage master listening on " << server_address;
}

void StorageMaster::Stop() {
  server_->Shutdown();
  LOG(INFO) << "Storage master shut down";
}

std::string StorageMaster::GenerateName(const IntroduceRequest& request) {
  switch (request.introduce_case()) {
  case IntroduceRequest::kStorageManager: {
    IntroduceRequest::StorageManagerIntroduce introduce = request.storage_manager();
    return "manager:" + StorageName_Name(introduce.storage_name()) + ":" +
           StorageType_Name(introduce.storage_type()) + ":" +
           introduce.rpc_hostname();
  }
  case IntroduceRequest::kStorageClient: {
    IntroduceRequest::StorageClientIntroduce introduce = request.storage_client();
    return "client:" + introduce.rpc_hostname();
  }
  case IntroduceRequest::INTRODUCE_NOT_SET: {
    return "INVALID";
  }
  }
  return "";
}

Status StorageMaster::Introduce(ServerContext * context,
                                const IntroduceRequest * request,
                                IntroduceReply * reply) {
  std::string name = GenerateName(*request);

  LOG(INFO) << "Received introduction from " << name;
  switch (request->introduce_case()) {
  case IntroduceRequest::kStorageManager: {
    IntroduceRequest::StorageManagerIntroduce introduce =
      request->storage_manager();

    std::string uri = introduce.rpc_hostname() + ":" + introduce.rpc_port();

    // Begin tracking peer
    peer_tracker_.AddPeer(name, uri, context->peer());

    // Add files to the file manager
    for (const auto& file_id : introduce.file()) {
      file_view_.AddFileToManager(name, file_id.key());
    }
    break;
  }
  case IntroduceRequest::kStorageClient: {
    IntroduceRequest::StorageClientIntroduce introduce = request->storage_client();
    std::string uri = introduce.rpc_hostname() + ":" + introduce.rpc_port();
    peer_tracker_.AddPeer(name, uri, context->peer());
    break;
  }
  case IntroduceRequest::INTRODUCE_NOT_SET: {
    break;
  }
  }
  reply->set_name(name);
  return Status::OK;
}

Status StorageMaster::Heartbeat(ServerContext * context,
                                const Empty * request,
                                Empty * reply) {
  LOG(INFO) << "Heartbeat received from " << context->peer();
  return Status::OK;
}

void StorageMaster::FillInRule(Rule* rule) {
  // Switch statement based on action types, then set mgr's uri field in action.
}

Status StorageMaster::InstallRule(ServerContext* context,
                                  const InstallRuleRequest* request,
                                  Empty* reply) {
  InstallRuleRequest new_request = *request;

  // Get uri of client
  // StorageMaster::PeerTracker::Peer p = peer_tracker_.GetPeerFromName(
  //                                        request->client());
  // std::string client_uri = p.rpc_uri;

  // Create stub to client
  // StorageClientInterface client()

  // Update rule request to fill in actions with mgr rpc uri's
  FillInRule(new_request.mutable_rule());

  // send rule request to client


  return Status::OK;
}

Status StorageMaster::RemoveRule(ServerContext* context,
                                 const RemoveRuleRequest* request,
                                 Empty* reply) {
  // Same flow as Install Rule
  return Status::OK;
}

Status StorageMaster::StorageChange(ServerContext* context,
                                    const StorageChangeRequest* request,
                                    Empty* reply) {

  for(const auto& change : request->storage_change()) {
    switch(change.op()) {
      case OperationType::PUT: {
        LOG(INFO) << "Added key: " << change.key() << " from " << change.manager();
        file_view_.AddFileToManager(change.manager(), change.key());
        break;
      }
      case OperationType::REMOVE: {
        LOG(INFO) << "Removed key: " << change.key() << " from " << change.manager();
        file_view_.RemoveFileFromManager(change.manager(), change.key());
        break;
      }
      default: {
        continue;
      }
    } 
  }
  return Status::OK;
}

Status StorageMaster::GetView(ServerContext* context,
                              const Empty* request,
                              GetViewReply* reply) {
  file_view_.PopulateViewReply(reply);
  return Status::OK;
}