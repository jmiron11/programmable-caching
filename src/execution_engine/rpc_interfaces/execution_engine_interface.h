#ifndef EXECUTION_ENGINE_INTERFACE_
#define EXECUTION_ENGINE_INTERFACE_

#include "proto/execution_engine.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Status;

class ExecutionEngineInterface {
 public:
	ExecutionEngineInterface(const std::string& uri);
	ExecutionEngineInterface(const std::string& hostname, const std::string& port);

	Status Execute(const Task& request);

 private:
	std::unique_ptr<ExecutionEngineService::Stub> stub_;
};

#endif