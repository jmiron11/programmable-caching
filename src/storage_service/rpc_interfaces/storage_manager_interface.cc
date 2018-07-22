#include "storage_manager_interface.h"
#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

StorageManagerInterface::StorageManagerInterface(const std::string& uri) {
	stub_ = StorageManagerService::NewStub(grpc::CreateChannel(
	    uri,
	    grpc::InsecureChannelCredentials()));
}

StorageManagerInterface::StorageManagerInterface(const std::string& hostname,
    const std::string& port) {
	stub_ = StorageManagerService::NewStub(grpc::CreateChannel(
	    hostname + ":" + port,
	    grpc::InsecureChannelCredentials()));
}

Status StorageManagerInterface::Put(const PutRequest& request) {
	Empty reply;
	ClientContext context;
	context.set_wait_for_ready(true);
	return stub_->Put(&context, request, &reply);
}

Status StorageManagerInterface::Get(const GetRequest& request,
                                    GetReply* reply) {
	ClientContext context;
	context.set_wait_for_ready(true);
	return stub_->Get(&context, request, reply);
}

Status StorageManagerInterface::Remove(const RemoveRequest& request) {
	Empty reply;
	ClientContext context;
	context.set_wait_for_ready(true);
	return stub_->Remove(&context, request, &reply);
}

Status StorageManagerInterface::CopyFrom(const CopyFromRequest& request) {
	Empty reply;
	ClientContext context;
	context.set_wait_for_ready(true);
	return stub_->CopyFrom(&context, request, &reply);
}