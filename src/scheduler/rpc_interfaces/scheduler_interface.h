#ifndef SCHEDULER_INTERFACE_H_
#define SCHEDULER_INTERFACE_H_

#include "proto/scheduler.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Status;

class SchedulerInterface {
 public:
	SchedulerInterface(const std::string& uri);
	SchedulerInterface(const std::string& hostname, const std::string& port);

	Status SubmitJob(const Job& request);
	
	Status Done(const Task& request);

	Status Introduce(const SchedulerIntroduceRequest& request);

 private:
	std::unique_ptr<SchedulerService::Stub> stub_;
};

#endif