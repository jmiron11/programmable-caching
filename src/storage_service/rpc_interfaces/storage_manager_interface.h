#ifndef STORAGE_MANAGER_INTERFACE_H_
#define STORAGE_MANAGER_INTERFACE_H_

#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Status;

class StorageManagerInterface {
 public:
	StorageManagerInterface(const std::string& uri);
	StorageManagerInterface(const std::string& hostname, const std::string& port);

	Status Put(const PutRequest& request);
	
	Status Get(const GetRequest& request, GetReply* reply);

	Status Remove(const RemoveRequest& request);

	Status CopyFrom(const CopyFromRequest& request);

 private:
	std::unique_ptr<StorageManagerService::Stub> stub_;
};


#endif