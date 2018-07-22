#include "storage_client.h"
#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;

StorageClient::StorageClient(const std::string& client_hostname,
                             const std::string& client_port,
                             const std::string& master_hostname,
                             const std::string& master_port):
	client_hostname_(client_hostname), client_port_(client_port),
	master_interface_(master_hostname, master_port) { }

void StorageClient::Start() {	
	// Introduction to the storage master.
	IntroduceRequest request;
	IntroduceRequest::StorageClientIntroduce* introduce =
	  request.mutable_storage_client();
	introduce->set_rpc_port(client_port_);
	introduce->set_rpc_hostname(client_hostname_);

	IntroduceReply reply;
	Status s = master_interface_.Introduce(request, &reply);

	std::string server_address(client_hostname_ + ':' + client_port_);
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(this);
	server_ = builder.BuildAndStart();
	LOG(INFO) << "Storage client listening on " << server_address;
}

void StorageClient::Stop() {
	server_->Shutdown();
	LOG(INFO) << "Storage client shut down";
}

Status StorageClient::Get(ServerContext* context,
                          const GetRequest* request,
                          GetReply* reply) {
	return Status::OK;
}

Status StorageClient::Put(ServerContext* context,
                          const PutRequest* request,
                          Empty* reply) {
	return Status::OK;
}

Status StorageClient::Remove(ServerContext* context,
                             const RemoveRequest* request,
                             Empty* reply) {
	return Status::OK;
}

Status StorageClient::InstallRule(ServerContext* context,
                                  const InstallRuleRequest* request,
                                  Empty* reply) {
	return Status::OK;
}

Status StorageClient::RemoveRule(ServerContext* context,
                                 const RemoveRuleRequest* request,
                                 Empty* reply) {
	return Status::OK;
}