#ifndef STORAGE_MASTER_INTERFACE_H_
#define STORAGE_MASTER_INTERFACE_H_

#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Status;


class StorageMasterInterface {
 public:
	StorageMasterInterface(const std::string& hostname,
	                       const std::string& port);

	Status IntroduceToMaster(const IntroduceRequest& request,
	                         IntroduceReply* reply);
	Status HeartbeatToMaster();

 private:
	std::unique_ptr<MasterService::Stub> stub_;
};

#endif