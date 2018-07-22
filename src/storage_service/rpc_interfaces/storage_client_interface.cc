#include "storage_client_interface.h"
#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

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