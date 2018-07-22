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

Status StorageMasterInterface::Introduce(
  const IntroduceRequest& request, IntroduceReply* reply) {
	ClientContext context;
	Status s = stub_->Introduce(&context, request, reply);
	return s;
}

Status StorageMasterInterface::Heartbeat() {
	Empty request, reply;
	ClientContext context;
	return stub_->Heartbeat(&context, request, &reply);
}

Status StorageMasterInterface::StorageChange(const StorageChangeRequest&
    request) {
	Empty reply;
	ClientContext context;
	return stub_->StorageChange(&context, request, &reply);

}

Status StorageMasterInterface::InstallRule(const InstallRuleRequest& request) {
	Empty reply;
	ClientContext context;
	return stub_->InstallRule(&context, request, &reply);

}

Status StorageMasterInterface::RemoveRule(const RemoveRuleRequest& request) {
	Empty reply;
	ClientContext context;
	return stub_->RemoveRule(&context, request, &reply);
}

Status StorageMasterInterface::GetView(GetViewReply * reply){
	Empty request;
	ClientContext context;
	return stub_->GetView(&context, request, reply);
}
