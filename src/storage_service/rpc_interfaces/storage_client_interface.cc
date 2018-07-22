#include "storage_client_interface.h"
#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

StorageClientInterface::StorageClientInterface(const std::string& uri) {
	stub_ = StorageClientService::NewStub(grpc::CreateChannel(
	                                        uri,
	                                        grpc::InsecureChannelCredentials()));
}

StorageClientInterface::StorageClientInterface(const std::string& hostname,
																						   const std::string& port) {
	stub_ = StorageClientService::NewStub(grpc::CreateChannel(
	                                        hostname + ":" + port,
	                                        grpc::InsecureChannelCredentials()));
}

Status StorageClientInterface::Put(const PutRequest& request) {
	Empty reply;
	ClientContext context;
	return stub_->Put(&context, request, &reply);
}

Status StorageClientInterface::Get(const GetRequest& request, GetReply* reply) {
	ClientContext context;
	return stub_->Get(&context, request, reply);
}

Status StorageClientInterface::Remove(const RemoveRequest& request) {
	Empty reply;
	ClientContext context;
	return stub_->Remove(&context, request, &reply);
}

Status StorageClientInterface::InstallRule(const InstallRuleRequest& request) {
	Empty reply;
	ClientContext context;
	return stub_->InstallRule(&context, request, &reply);
}

Status StorageClientInterface::RemoveRule(const RemoveRuleRequest& request) {
	Empty reply;
	ClientContext context;
	return stub_->RemoveRule(&context, request, &reply);
}