#ifndef EXECUTION_ENGINE_
#define EXECUTION_ENGINE_

#include "proto/execution_engine.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Status;
using grpc::ServerContext;
using grpc::Server;

class ExecutionEngine final : public ExecutionEngineService::Service {
 public:
	ExecutionEngine(const std::string& execution_engine_hostname,
	                const std::string& execution_engine_port,
	                const std::string& storage_client_hostname,
	                const std::string& storage_client_port,
	                const std::string& scheduler_hostname,
	                const std::string& scheduler_port);
	~ExecutionEngine() { }

	void Start();
	void Stop();

	Status Execute(ServerContext* context,
	                 const Task* request,
	                 Empty* reply) override;

 private:
};

#endif