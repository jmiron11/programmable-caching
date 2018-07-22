#include "execution_engine.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

#include <string>
#include <chrono>

using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using std::string;


ExecutionEngine::ExecutionEngine(const std::string& execution_engine_hostname,
                                 const std::string& execution_engine_port,
                                 const std::string& storage_client_hostname,
                                 const std::string& storage_client_port,
                                 const std::string& scheduler_hostname,
                                 const std::string& scheduler_port): 
execution_engine_hostname_(execution_engine_hostname),
execution_engine_port_(execution_engine_port),
scheduler_interface_(scheduler_hostname, scheduler_port),
storage_client_interface_(storage_client_hostname, storage_client_port) {

}

void ExecutionEngine::Start() {
	// TODO(justinmiron): Introduction to scheduler via SchedulerInterface


	// Begin processing of storage management RPCs.
	std::string server_address(execution_engine_hostname_ + ':' +
	                           execution_engine_port_);
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(this);
	server_ = builder.BuildAndStart();
	LOG(INFO) << "Scheduler listening on " << server_address;
}

void ExecutionEngine::Stop() {
	server_->Shutdown();
	LOG(INFO) << "Scheduler shutdown";
}

Status ExecutionEngine::Execute(ServerContext* context,
                                const Task* request,
                                Empty* reply) {
	return Status::OK;
}
