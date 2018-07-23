#include "scheduler.h"

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

Scheduler::Scheduler(const std::string& scheduler_hostname,
                     const std::string& scheduler_port,
                     const std::string& storage_hostname,
                     const std::string& storage_port):
	storage_interface_(storage_hostname, storage_port),
	scheduler_hostname_(scheduler_hostname),
	scheduler_port_(scheduler_port),
	system_view_(StorageName::EPHEMERAL, 0, StorageName::FAKE_PERSISTANT, 1) {
}

void Scheduler::Start() {
	// Begin processing of storage management RPCs.
	std::string server_address(scheduler_hostname_ + ':' + scheduler_port_);
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(this);
	server_ = builder.BuildAndStart();
	LOG(INFO) << "Scheduler listening on " << server_address;
}

void Scheduler::Stop() {
	server_->Shutdown();
	LOG(INFO) << "Scheduler shutdown";
}

Status Scheduler::SubmitJob(ServerContext* context,
                            const Job* request,
                            Empty* reply) {

	// Add job to the system state

	return Status::OK;
}

Status Scheduler::Done(ServerContext* context,
                       const Task* request,
                       Empty* reply) {

	// Remove task from the system state

	return Status::OK;
}

Status Scheduler::Introduce(ServerContext* context,
                            const SchedulerIntroduceRequest* request,
                            Empty* reply) {
	// Named after rpc_uri.
	system_view_.AddExecutionEngine(request->rpc_uri(), request->client_rpc_uri());

	return Status::OK;
}

// void Scheduler::Schedule(const SystemState& system_snapshot,
//                          const FileState& file_snapshot, 
//                          SchedulingDecisions* decisions) {

// }

void Scheduler::SchedulerControlThread() {
	// Get global view of the system.

	// Snapshot the system state.

	// Call schedule function.

	// Run execution of scheduling decisions.
}