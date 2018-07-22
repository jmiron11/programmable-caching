#ifndef STORAGE_MANAGER_H_
#define STORAGE_MANAGER_H_

#include "proto/storage_service.grpc.pb.h"
#include "storage_service/storage_interfaces/storage_interface.h"
#include "storage_service/rpc_interfaces/storage_master_interface.h"

#include <string>
#include <thread>

using grpc::Status;
using grpc::ServerContext;

class StorageManager : public StorageManagerService::Service {
 public:
	StorageManager(const std::string& manager_hostname,
	               const std::string& manager_port,
	               const std::string& master_hostname,
	               const std::string& master_port,
	               int master_heartbeat_interval_seconds);
	~StorageManager();

	void Start();

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

 protected:
	// Storage interface for accesss to the managed storage medium.
	std::unique_ptr<StorageInterface> storage_interface_;

 private:
	// Responsible for initializing connection to storage medium and starting up
	// server thread.
	void ManageStorage(const std::string& hostname, const std::string& port);

	/* Interface and thread for communicating with master, this involves
	   introductions and regular heartbeating. */

	// Function executed in a thread that introduces storage manager to master,
	// and regularly heartbeats at heartbeat_interval
	void IntroduceAndHeartbeat(int heartbeat_interval,
	                           std::string manager_port,
	                           std::string manager_host);

	// Interface for RPCs with master.
	StorageMasterInterface master_interface_;

	// Thread that regularly heartbeats master
	// TODO(justinmiron): Fragment heartbeating from storage manager into node
	// health monitor.
	std::thread heartbeat_thread_;

	std::string name_;
	std::string manager_hostname_;
	std::string manager_port_;
	std::string master_hostname_;
	std::string master_port_;
	int master_heartbeat_interval_seconds_;
};


#endif  // STORAGE_MANAGER_H_