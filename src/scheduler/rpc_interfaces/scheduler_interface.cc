#include "scheduler_interface.h"

#include <grpcpp/grpcpp.h>

using grpc::ClientContext;

SchedulerInterface::SchedulerInterface(const std::string& uri) {
	stub_ = SchedulerService::NewStub(grpc::CreateChannel(
	                                        uri,
	                                        grpc::InsecureChannelCredentials()));
}

SchedulerInterface::SchedulerInterface(const std::string& hostname, const std::string& port) {
	stub_ = SchedulerService::NewStub(grpc::CreateChannel(
	                                        hostname + ":" + port,
	                                        grpc::InsecureChannelCredentials()));
}

Status SchedulerInterface::SubmitJob(const Job& request) {
	Empty reply;
	ClientContext context;
	return stub_->SubmitJob(&context, request, &reply);
}

Status SchedulerInterface::Done(const Task& request) {
	Empty reply;
	ClientContext context;
	return stub_->Done(&context, request, &reply);
}

Status SchedulerInterface::Introduce(const SchedulerIntroduceRequest& request) {
	Empty reply;
	ClientContext context;
	return stub_->Introduce(&context, request, &reply);
}

