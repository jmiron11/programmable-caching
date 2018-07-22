#ifndef STORAGE_CLIENT_INTERFACE_H_
#define STORAGE_CLIENT_INTERFACE_H_

#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Status;

class StorageClientInterface {
 public:
	StorageClientInterface(const std::string& hostname,
	                       const std::string& port);

	Status Put(const PutRequest& request);
	
	Status Get(const GetRequest& request, GetReply* reply);

	Status Remove(const RemoveRequest& request);

	Status InstallRule(const InstallRuleRequest& request);

	Status RemoveRule(const RemoveRuleRequest& request);

 private:
	std::unique_ptr<StorageClientService::Stub> stub_;
};

#endif