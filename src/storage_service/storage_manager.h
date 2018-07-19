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

		Status IntroduceToMaster(StorageType type);
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

 protected:
 	// Storage interface for accesss to the managed storage medium.
	std::unique_ptr<StorageInterface> storage_interface_;

 private:
 	// TODO(justinmiron): Initialize data structures to store mapping from
 	// hostname:port of other storage managers. If UNMANAGED data source,
 	// use information received with RPC from master to initialize wrapper as
 	// well. Protect data structures accessed by RPCs with concurrency control.


	// Responsible for initializing connection to storage medium and starting up
	// server thread.
	void ManageStorage(const std::string& hostname, const std::string& port);

	/* Interface and thread for communicating with master, this involves
	   introductions and regular heartbeating. */

	// Function executed in a thread that introduces storage manager to master,
	// and regularly heartbeats at heartbeat_interval
	void IntroduceAndHeartbeat(int heartbeat_interval);

	// Interface for RPCs with master.
	StorageMasterInterface master_interface_;

	// Thread that regularly heartbeats master
	// TODO(justinmiron): Fragment heartbeating from storage manager into node
	// health monitor.
	std::thread heartbeat_thread_;
};


#endif  // STORAGE_MANAGER_H_