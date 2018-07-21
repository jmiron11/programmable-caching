#include "storage_master_interface.h"
#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


StorageMasterInterface::StorageMasterInterface(
  const std::string& hostname, const std::string& port) {
	stub_ = MasterService::NewStub(grpc::CreateChannel(
	                                 hostname + ":" + port,
	                                 grpc::InsecureChannelCredentials()));
}

Status StorageMasterInterface::IntroduceToMaster(
  const IntroduceRequest& request, IntroduceReply* reply) {
	ClientContext context;
	Status s = stub_->Introduce(&context, request, reply);
	return s;
}

Status StorageMasterInterface::HeartbeatToMaster() {
	Empty request, reply;
	ClientContext context;
	return stub_->Heartbeat(&context, request, &reply);
}