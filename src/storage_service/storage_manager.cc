#include "storage_manager.h"
#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

#include <string>
#include <thread>
#include <chrono>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using std::string;

StorageManager::StorageMasterInterface::StorageMasterInterface(
  const std::string& hostname, const std::string& port) {
	stub_ = MasterService::NewStub(grpc::CreateChannel(
	                                 hostname + ":" + port,
	                                 grpc::InsecureChannelCredentials()));
}

Status StorageManager::StorageMasterInterface::IntroduceToMaster(
  StorageType type, StorageName name, const std::string& port,
  const std::string& hostname, std::string* assigned_name) {
	IntroduceRequest request;
	request.set_storage_type(type);
	request.set_storage_name(name);
	request.set_rpc_port(port);
	request.set_rpc_hostname(hostname);

	IntroduceReply reply;
	ClientContext context;
	Status s = stub_->Introduce(&context, request, &reply);
	*assigned_name = reply.name();
	return s;
}

Status StorageManager::StorageMasterInterface::HeartbeatToMaster() {
	HeartbeatRequest request;
	HeartbeatReply reply;
	ClientContext context;
	return stub_->Heartbeat(&context, request, &reply);
}

StorageManager::StorageManager(const std::string& manager_hostname,
                               const std::string& manager_port,
                               const std::string& master_hostname,
                               const std::string& master_port,
                               int master_heartbeat_interval_seconds)
	: master_interface_(master_hostname, master_port),
	  manager_hostname_(manager_hostname),
	  manager_port_(manager_port),
	  master_hostname_(master_hostname),
	  master_port_(master_port),
	  master_heartbeat_interval_seconds_(master_heartbeat_interval_seconds) { }

void StorageManager::Start() {
	heartbeat_thread_ = std::thread(&StorageManager::IntroduceAndHeartbeat, this,
	                                master_heartbeat_interval_seconds_, manager_port_,
	                                manager_hostname_);
	ManageStorage(manager_hostname_, manager_port_);
}

StorageManager::~StorageManager() {
	heartbeat_thread_.join();
}

void StorageManager::ManageStorage(const std::string& hostname,
                                   const std::string& port) {
	// Begin processing of storage management RPCs.
	std::string server_address(hostname + ':' + port);

	ServerBuilder builder;
	// Listen on the given address without any authentication mechanism.
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	// Register "service" as the instance through which we'll communicate with
	// clients. In this case it corresponds to an *synchronous* service.
	builder.RegisterService((StorageManager*)this);
	// Finally assemble the server.
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;

	// Wait for the server to shutdown. Note that some other thread must be
	// responsible for shutting down the server for this call to ever return.
	server->Wait();
}

void StorageManager::IntroduceAndHeartbeat(int heartbeat_interval,
    std::string manager_port, std::string manager_host) {
	Status s = master_interface_.IntroduceToMaster(
	             storage_interface_->GetStorageType(),
	             storage_interface_->GetStorageName(),
	             manager_port, manager_host, &name_);

	// TODO(justinmiron): Implement better error handling on failure to introduce.
	if (s.ok())
		LOG(INFO) << "Successfully introduced to master";
	else
		LOG(WARNING) << "Error introducing to master";

	auto curr_t = std::chrono::system_clock::now();

	while (1) {
		Status s = master_interface_.HeartbeatToMaster();
		if (s.ok())
			LOG(INFO) << "To the beat of a drum";
		else
			LOG(WARNING) << "Error heartbeating, this is how it ends.";

		auto next_t = curr_t + std::chrono::seconds{heartbeat_interval};
		std::this_thread::sleep_until(next_t);
		curr_t = next_t;
	}
}

void StorageManager::OffloadDisseminateStorageManagers(
  std::vector<std::pair<std::string, std::string>> name_uri) {

	for(auto& mgr : name_uri) {
		if (mgr.first != name_) {
			manager_view.Add(mgr.first, mgr.second);
		}
	}
}

Status StorageManager::DisseminateStorageManagers(ServerContext* context,
    const DisseminateRequest* request,
    DisseminateReply* reply) {
	LOG(INFO) << "Got dissemination!";
	std::vector<std::pair<std::string, std::string>> name_uris;

	for (auto& mgr : request->delta())	{
		name_uris.push_back(std::pair<std::string, std::string>(mgr.name(), mgr.uri()));
	}

	std::thread t(&StorageManager::OffloadDisseminateStorageManagers, this,
	              name_uris);
	t.detach();

	return Status::OK;
}

Status StorageManager::Get(ServerContext* context,
                           const GetRequest* request,
                           GetReply* reply) {
	return Status::OK;
}

Status StorageManager::Put(ServerContext* context,
                                const PutRequest* request,
                                PutReply* reply) {
	// Puts file in database
	// Send RPC to master informing of operation
	return Status::OK;
}

Status StorageManager::Remove(ServerContext* context,
                                const RemoveRequest* request,
                                RemoveReply* reply) {
	// Remove file from db
	// Send RPC to master informing of operation
	return Status::OK;
}

Status StorageManager::CopyFrom(ServerContext* context,
                                const CopyFromRequest* request,
                                CopyFromReply* reply) {
	return Status::OK;
}