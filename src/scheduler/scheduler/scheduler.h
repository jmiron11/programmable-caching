#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "job_view.h"
#include "scheduler_common.h"
#include "proto/scheduler.grpc.pb.h"
#include "storage_service/rpc_interfaces/storage_master_interface.h"
#include "serverless_system_view.h"

#include <grpcpp/grpcpp.h>
#include <string>
#include <thread>

using grpc::Status;
using grpc::ServerContext;
using grpc::Server;

class Scheduler final : public SchedulerService::Service {
 public:
	Scheduler(const std::string& scheduler_hostname,
	          const std::string& scheduler_port,
	          const std::string& storage_hostname, const std::string& storage_port,
	          int scheduler_interval_seconds);
	~Scheduler() { }

	void Start();
	void Stop();

	// Scheduling of jobs
	void SchedulerControlThread();

	// Constructs scheduling decisions
	// Inputs to scheduler: ExecutionEngine,

	// Get scheduling decisions
	virtual void Schedule(std::vector<std::string> pending_jobs,
	                      SchedulingDecisions* decisions);

	// Execution of scheduling decisions
	void Execute(const SchedulingDecisions& decisions);

	Status SubmitJob(ServerContext* context,
	                 const Job* request,
	                 Empty* reply) override;

	Status Done(ServerContext* context,
	            const Task* request,
	            Empty* reply) override;

	Status Introduce(ServerContext* context,
	                 const SchedulerIntroduceRequest* request,
	                 Empty* reply) override;

 protected:
	// Maintains a view of the engines and the files stored ephemerally.
	ServerlessSystemView system_view_;

	// Maintains a view of the jobs (pending and running) in the system.
	JobView job_view_;

 private:
	StorageMasterInterface storage_interface_;

	std::string scheduler_hostname_;
	std::string scheduler_port_;
	std::unique_ptr<Server> server_;

	std::thread scheduler_thread_;

	std::mutex scheduler_mutex_;
	bool running_;
	int scheduler_interval_seconds_;
};

#endif