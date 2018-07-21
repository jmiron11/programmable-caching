
#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <thread>
#include <chrono>

#include "proto/storage_service.grpc.pb.h"
#include "storage_master.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;

std::string StorageMaster::PeerTracker::GetUri(const std::string& name) {
  std::lock_guard<std::mutex> lock(tracker_mutex);
  return name_to_uri_[name];
}

void StorageMaster::PeerTracker::AddPeer(const std::string& name, const std::string& uri) {
  std::lock_guard<std::mutex> lock(tracker_mutex);
  name_to_uri_[name] = uri;
}

StorageMaster::StorageMaster(const std::string& hostname,
                             const std::string& port) {

  std::string server_address(hostname + ':' + port);
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(this);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
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
    IntroduceRequest::StorageManagerIntroduce introduce = request->storage_manager();
    std::string uri = introduce.rpc_hostname() + ":" + introduce.rpc_port();
    peer_tracker_.AddPeer(name, uri);
    break;
  }
  case IntroduceRequest::kStorageClient: {
    IntroduceRequest::StorageClientIntroduce introduce = request->storage_client();
    std::string uri = introduce.rpc_hostname() + ":" + introduce.rpc_port();
    peer_tracker_.AddPeer(name, uri);
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
  LOG(INFO) << "heartbeat received from " << context->peer();
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
  std::string client_uri = peer_tracker_.GetUri(request->client());

  // Create stub to client

  // Update rule request to fill in actions with mgr details
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