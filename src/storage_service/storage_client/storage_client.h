#ifndef STORAGE_CLIENT_H_
#define STORAGE_CLIENT_H_

#include "proto/storage_service.grpc.pb.h"
#include "storage_service/rpc_interfaces/storage_master_interface.h"
#include "rule_db.h"

#include <grpcpp/grpcpp.h>
#include <string>

using grpc::Status;
using grpc::ServerContext;
using grpc::Server;

class StorageClient final : public StorageClientService::Service {
 public:
	StorageClient(const std::string& manager_hostname,
	              const std::string& manager_port,
	              const std::string& master_hostname,
	              const std::string& master_port);

	void Start();
	void Stop();

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

	void ExecuteGetAction(const Rule::Action::GetAction& action, GetReply* reply);

	void ExecuteCopyAction(const Rule::Action::CopyFromAction& action);

	void ExecutePutAction(const Rule::Action::PutAction& action,
	                      const PutRequest* request);

	void ExecuteRemoveAction(const Rule::Action::RemoveAction& action);

	RuleDb rule_db_;

	const std::string client_hostname_;
	const std::string client_port_;
	StorageMasterInterface master_interface_;

	std::unique_ptr<Server> server_;

};

#endif