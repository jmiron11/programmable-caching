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
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService((StorageManager*)this);
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();
}

void StorageManager::IntroduceAndHeartbeat(int heartbeat_interval,
    std::string manager_port, std::string manager_host) {

	//Construct the introduce request and send it to the master
	IntroduceRequest request;
	IntroduceRequest::StorageManagerIntroduce* introduce =
	  request.mutable_storage_manager();
	introduce->set_storage_type(storage_interface_->GetStorageType());
	introduce->set_storage_name(storage_interface_->GetStorageName());
	introduce->set_rpc_port(manager_port);
	introduce->set_rpc_hostname(manager_host);

	std::vector<std::string> keys;
	storage_interface_->GetAllKeys(&keys);

	for(const auto& key : keys) {
		IntroduceRequest::StorageManagerIntroduce::FileId * f = introduce->add_file();
		f->set_key(key);
	}

	IntroduceReply reply;
	Status s = master_interface_.Introduce(request, &reply);
	name_ = reply.name();

	// TODO(justinmiron): Implement better error handling on failure to introduce.
	if (s.ok())
		LOG(INFO) << "Successfully introduced to master";
	else
		LOG(WARNING) << "Error introducing to master";

	auto curr_t = std::chrono::system_clock::now();

	while (1) {
		Status s = master_interface_.Heartbeat();
		if (s.ok())
			LOG(INFO) << "To the beat of a drum";
		else
			LOG(WARNING) << "Error heartbeating, this is how it ends.";

		auto next_t = curr_t + std::chrono::seconds{heartbeat_interval};
		std::this_thread::sleep_until(next_t);
		curr_t = next_t;
	}
}

Status StorageManager::Get(ServerContext* context,
                           const GetRequest* request,
                           GetReply* reply) {
	std::string* data = reply->mutable_value();
	StorageInterface::Status s = storage_interface_->Get(request->key(), data);
	if (s != StorageInterface::OK) {
		LOG(ERROR) << "Error get'ing value associated with " << request->key() <<
		           "from storage medium";
	}
	return Status::OK;
}

Status StorageManager::Put(ServerContext* context,
                           const PutRequest* request,
                           Empty* reply) {
	// Puts file in database
	// Send RPC to master informing of operation
	StorageInterface::Status s = storage_interface_->Put(request->key(),
	                             request->value());
	if (s != StorageInterface::OK) {
		LOG(ERROR) << "Error put'ing value associated with " << request->key() <<
		           "from storage medium";
	}
	return Status::OK;
}

Status StorageManager::Remove(ServerContext* context,
                              const RemoveRequest* request,
                              Empty* reply) {
	// Remove file from db
	// Send RPC to master informing of operation
	StorageInterface::Status s = storage_interface_->Remove(request->key());
	if (s != StorageInterface::OK) {
		LOG(ERROR) << "Error removing value associated with " << request->key() <<
		           "from storage medium";
	}
	return Status::OK;
}

Status StorageManager::CopyFrom(ServerContext* context,
                                const CopyFromRequest* request,
                                Empty* reply) {
	std::unique_ptr<StorageManagerService::Stub> stub
	  = StorageManagerService::NewStub(grpc::CreateChannel(
	                                     request->dst_uri(),
	                                     grpc::InsecureChannelCredentials()));
	GetRequest get_request;
	get_request.set_key(request->key());
	GetReply get_reply;
	ClientContext ctx;

	stub->Get(&ctx, get_request, &get_reply);
	storage_interface_->Put(get_request.key(), get_reply.value());
	return Status::OK;
}
