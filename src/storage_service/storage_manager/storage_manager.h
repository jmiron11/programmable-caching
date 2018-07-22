#ifndef STORAGE_MANAGER_H_
#define STORAGE_MANAGER_H_

#include "proto/storage_service.grpc.pb.h"
#include "storage_service/storage_interfaces/storage_interface.h"
#include "storage_service/rpc_interfaces/storage_master_interface.h"

#include <string>

using grpc::Status;
using grpc::ServerContext;
using grpc::Server;

class StorageManager : public StorageManagerService::Service {
 public:
	StorageManager(const std::string& manager_hostname,
	               const std::string& manager_port,
	               const std::string& master_hostname,
	               const std::string& master_port,
	               std::unique_ptr<StorageInterface> storage_interface);

	void Start();
	
	void Stop();

	// Storage manager RPCs.
	Status CopyFrom(ServerContext* context, const CopyFromRequest* request,
	                Empty* reply) override;

	Status Get(ServerContext* context,
	           const GetRequest* request,
	           GetReply* reply) override;

	Status Put(ServerContext* context,
	           const PutRequest* request,
	           Empty* reply) override;

	Status Remove(ServerContext* context,
	           const RemoveRequest* request,
	           Empty* reply) override;

 private:
	// Interface for RPCs with master.
	StorageMasterInterface master_interface_;

	std::string name_;
	std::string manager_hostname_;
	std::string manager_port_;

	std::unique_ptr<StorageInterface> storage_interface_;
	std::unique_ptr<Server> server_;
};


#endif  // STORAGE_MANAGER_H_