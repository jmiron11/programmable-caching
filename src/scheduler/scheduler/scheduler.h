#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "proto/scheduler.grpc.pb.h"
#include "storage_service/rpc_interfaces/storage_master_interface.h"

#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Status;
using grpc::ServerContext;
using grpc::Server;

class Scheduler final : public SchedulerService::Service {
 public:
	Scheduler(const std::string& scheduler_hostname, const std::string& scheduler_port,
						const std::string& storage_hostname, const std::string& storage_port);
	~Scheduler(){ }

	void Start();
	void Stop();
	
	Status SubmitJob(ServerContext* context,
	           const Job* request,
	           Empty* reply) override;

	Status Done(ServerContext* context,
	           const Task* request,
	           Empty* reply) override;

	Status Introduce(ServerContext* context,
                 const SchedulerIntroduceRequest* request,
                 Empty* reply) override;


 private:
 	StorageMasterInterface storage_interface_;

 	std::string scheduler_hostname_;
	std::string scheduler_port_;
	std::unique_ptr<Server> server_;
};

#endif