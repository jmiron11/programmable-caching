#ifndef STORAGE_CLIENT_H_
#define STORAGE_CLIENT_H_

#include <grpcpp/grpcpp.h>
#include <string>

#include "proto/storage_service.grpc.pb.h"
#include "storage_service/rpc_interfaces/storage_master_interface.h"

using grpc::Status;
using grpc::ServerContext;

class StorageClient final : public StorageClientService::Service {
 public:
	StorageClient(const std::string& manager_hostname,
	              const std::string& manager_port,
	              const std::string& master_hostname,
	              const std::string& master_port);

	Status Get(ServerContext* context,
	           const GetRequest* request,
	           GetReply* reply) override;

	Status Put(ServerContext* context,
	           const PutRequest* request,
	           Empty* reply) override;

	Status Remove(ServerContext* context,
	              const RemoveRequest* request,
	              Empty* reply) override;

	Status InstallRule(ServerContext* context,
	                   const InstallRuleRequest* request,
	                   Empty* reply) override;

	Status RemoveRule(ServerContext* context,
	                  const RemoveRuleRequest* request,
	                  Empty* reply) override;

 private:
 	StorageMasterInterface master_interface_;
};

#endif