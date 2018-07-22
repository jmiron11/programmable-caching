#include "execution_engine_interface.h"
#include "proto/execution_engine.grpc.pb.h"

#include <grpcpp/grpcpp.h>

using grpc::ClientContext;
using grpc::Status;

ExecutionEngineInterface::ExecutionEngineInterface(const std::string& uri) {
	stub_ = ExecutionEngineService::NewStub(grpc::CreateChannel(
	                                        uri,
	                                        grpc::InsecureChannelCredentials()));
}

ExecutionEngineInterface::ExecutionEngineInterface(const std::string& hostname, const std::string& port) {
	stub_ = ExecutionEngineService::NewStub(grpc::CreateChannel(
	                                        hostname + ":" + port,
	                                        grpc::InsecureChannelCredentials()));
}

Status ExecutionEngineInterface::Execute(const Task& request) {
	Empty reply;
	ClientContext context;
	return stub_->Execute(&context, request, &reply);
}
