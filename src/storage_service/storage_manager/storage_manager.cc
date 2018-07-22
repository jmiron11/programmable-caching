#include "storage_manager.h"
#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

#include <string>
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
                               std::unique_ptr<StorageInterface> storage_interface)
	: master_interface_(master_hostname, master_port),
	  manager_hostname_(manager_hostname),
	  manager_port_(manager_port),
	  storage_interface_(std::move(storage_interface)) 
	  { }

void StorageManager::Start() {
	IntroduceRequest request;
	IntroduceRequest::StorageManagerIntroduce* introduce =
	  request.mutable_storage_manager();
	introduce->set_storage_type(storage_interface_->GetStorageType());
	introduce->set_storage_name(storage_interface_->GetStorageName());
	introduce->set_rpc_port(manager_port_);
	introduce->set_rpc_hostname(manager_hostname_);

	std::vector<std::string> keys;
	storage_interface_->GetAllKeys(&keys);

	for (const auto& key : keys) {
		IntroduceRequest::StorageManagerIntroduce::FileId * f = introduce->add_file();
		f->set_key(key);
	}

	IntroduceReply reply;
	Status s = master_interface_.Introduce(request, &reply);
	assert(s.ok());
	name_ = reply.name();

	// Begin processing of storage management RPCs.
	std::string server_address(manager_hostname_ + ':' + manager_port_);
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(this);
	server_ = builder.BuildAndStart();
	LOG(INFO) << "Storage manager listening on " << server_address;
}

void StorageManager::Stop() {
	server_->Shutdown();
	LOG(INFO) << "Storage manager shutdown";
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
