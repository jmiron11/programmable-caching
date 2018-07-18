#ifndef STORAGE_MANAGER_H_
#define STORAGE_MANAGER_H_

#include "proto/storage_service.grpc.pb.h"
#include "storage_interface.h"

#include <string>
#include <thread>

using grpc::Status;
using grpc::ServerContext;

class StorageManager : public StorageManagerService::Service {
 public:
	// Storage Manager's RPC interface with the storage master.
	class StorageMasterInterface {
	 public:
		StorageMasterInterface(const std::string& hostname,
		                       const std::string& port);

		Status IntroduceToMaster();
		Status HeartbeatToMaster();

	 private:
		std::unique_ptr<MasterService::Stub> stub_;
	};

	StorageManager(const std::string& manager_hostname,
	               const std::string& manager_port,
	               const std::string& master_hostname,
	               const std::string& master_port,
	               int master_heartbeat_interval_seconds);
	~StorageManager();

	// Storage manager RPCs.
	Status CopyFrom(ServerContext* context, const CopyFromRequest* request,
	                CopyFromReply* reply) override;

 private:
	// Responsible for initializing connection to storage medium and starting up
	// server thread.
	void ManageStorage(const std::string& hostname, const std::string& port);

	void InitializeAndHandleRPCs(const std::string& hostname,
	                             const std::string& port);

	StorageInterface* storage_interface_;

	// Interface and thread for communicating with master, this involves
	// introductions and regular heartbeating.

	// Function executed in a thread that introduces storage manager to master,
	// and regularly heartbeats at heartbeat_interval
	void IntroduceAndHeartbeat(int heartbeat_interval);

	StorageMasterInterface master_interface_;
	std::thread heartbeat_thread_;
};


#endif  // STORAGE_MANAGER_H_