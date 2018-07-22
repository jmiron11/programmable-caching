#include <string>

#include "storage_client.h"
#include "proto/storage_service.grpc.pb.h"

using grpc::Status;
using grpc::ServerContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ClientContext;

StorageClient::StorageClient(const std::string& manager_hostname,
                             const std::string& manager_port,
                             const std::string& master_hostname,
                             const std::string& master_port):
	master_interface_(master_hostname, master_port) {

	// Introduction to the storage master.
	IntroduceRequest request;
	IntroduceRequest::StorageClientIntroduce* introduce =
	request.mutable_storage_client();
	introduce->set_rpc_port(manager_port);
	introduce->set_rpc_hostname(manager_hostname);

	IntroduceReply reply;
	Status s = master_interface_.Introduce(request, &reply);

	std::string server_address(manager_hostname + ':' + manager_port);
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(this);
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Server listening on " << server_address << std::endl;
	server->Wait();
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