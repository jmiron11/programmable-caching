#include "storage_client.h"
#include "proto/storage_service.grpc.pb.h"
#include "storage_service/rpc_interfaces/storage_manager_interface.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;

StorageClient::StorageClient(const std::string& client_hostname,
                             const std::string& client_port,
                             const std::string& master_hostname,
                             const std::string& master_port):
	client_hostname_(client_hostname), client_port_(client_port),
	master_interface_(master_hostname, master_port) { }

void StorageClient::Start() {
	// Introduction to the storage master.
	IntroduceRequest request;
	IntroduceRequest::StorageClientIntroduce* introduce =
	  request.mutable_storage_client();
	introduce->set_rpc_port(client_port_);
	introduce->set_rpc_hostname(client_hostname_);

	IntroduceReply reply;
	Status s = master_interface_.Introduce(request, &reply);

	std::string server_address(client_hostname_ + ':' + client_port_);
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(this);
	server_ = builder.BuildAndStart();
	LOG(INFO) << "Storage client listening on " << server_address;
}

void StorageClient::Stop() {
	server_->Shutdown();
	LOG(INFO) << "Storage client shut down";
}

void StorageClient::ExecuteGetAction(const Rule::Action::GetAction& action,
                                     GetReply* reply) {
	GetRequest req;
	req.set_key(action.key());

	StorageManagerInterface manager_interface(action.mgr().uri());
	manager_interface.Get(req, reply);	
}

void StorageClient::ExecuteCopyAction(const Rule::Action::CopyFromAction&
                                      action) {
	CopyFromRequest req;
	req.set_key(action.key());
	req.set_dst_uri(action.dst_mgr().uri());

	StorageManagerInterface manager_interface(action.src_mgr().uri());
	manager_interface.CopyFrom(req);
}

void StorageClient::ExecutePutAction(const Rule::Action::PutAction& action,
                                     const PutRequest* request ) {
	StorageManagerInterface manager_interface(action.mgr().uri());
	manager_interface.Put(*request);
}

void StorageClient::ExecuteRemoveAction(const Rule::Action::RemoveAction&
                                        action) {
	RemoveRequest req;
	req.set_key(action.key());	
	StorageManagerInterface manager_interface(action.mgr().uri());
	manager_interface.Remove(req);
}

Status StorageClient::Get(ServerContext* context,
                          const GetRequest* request,
                          GetReply* reply) {
	// Get is received, check for rule (currently there must be a rule already).
	assert(rule_db_.HasMatchingRule(request->key(), OperationType::GET));
	Rule r = rule_db_.GetMatchingRule(request->key(), OperationType::GET);

	// Perform all actions except the last action.
	for (int action_idx = 0; action_idx < r.action_size() - 1; ++action_idx) {
		Rule::Action current_action = r.action(action_idx);

		switch (current_action.action_type_case()) {
		case Rule::Action::kGetAction: {
			LOG(ERROR) << "GetAction is not allowed except as last action";
			assert(false);
			break;
		}
		case Rule::Action::kCopyFromAction: {
			ExecuteCopyAction(current_action.copy_from_action());
			break;
		}
		case Rule::Action::kRemoveAction: {
			ExecuteRemoveAction(current_action.remove_action());
		}
		case Rule::Action::kPutAction: {
			// Put action is meaningless here
			LOG(ERROR) << "PutAction is not allowed for get requests";
			assert(false);
			break;
		}
		case Rule::Action::ACTION_TYPE_NOT_SET: {
			LOG(ERROR) << "Ill-formed action missing action type";
			break;
		}
		}
	}

	// Assert that last action has OperationType::GET
	Rule::Action last_action = r.action(r.action_size() - 1);
	assert(last_action.has_get_action());
	assert(last_action.get_action().key() == request->key());

	ExecuteGetAction(last_action.get_action(), reply);
	return Status::OK;
}

Status StorageClient::Put(ServerContext* context,
                          const PutRequest* request,
                          Empty* reply) {
	// Get is received, check for rule (currently there must be a rule already).
	assert(rule_db_.HasMatchingRule(request->key(), OperationType::GET));
	Rule r = rule_db_.GetMatchingRule(request->key(), OperationType::GET);

	// Perform all actions except the last action.
	for (int action_idx = 0; action_idx < r.action_size() - 1; ++action_idx) {
		Rule::Action current_action = r.action(action_idx);

		switch (current_action.action_type_case()) {
		case Rule::Action::kGetAction: {
			LOG(ERROR) << "GetAction is meaningless for Put operations";
			assert(false);
			break;
		}
		case Rule::Action::kCopyFromAction: {
			ExecuteCopyAction(current_action.copy_from_action());
			break;
		}
		case Rule::Action::kRemoveAction: {
			ExecuteRemoveAction(current_action.remove_action());
		}
		case Rule::Action::kPutAction: {
			assert(current_action.put_action().key() == request->key());
			ExecutePutAction(current_action.put_action(), request);
			break;
		}
		case Rule::Action::ACTION_TYPE_NOT_SET: {
			LOG(ERROR) << "Ill-formed aciton missing action type";
			break;
		}
		}
	}
	return Status::OK;

}

Status StorageClient::Remove(ServerContext* context,
                             const RemoveRequest* request,
                             Empty* reply) {

	// No implementation currently, no real need for remove in system.
	return Status::OK;
}

Status StorageClient::InstallRule(ServerContext* context,
                                  const InstallRuleRequest* request,
                                  Empty* reply) {

	rule_db_.AddRule(request->rule());
	return Status::OK;
}

Status StorageClient::RemoveRule(ServerContext* context,
                                 const RemoveRuleRequest* request,
                                 Empty* reply) {
	rule_db_.RemoveRule(request->rule());
	return Status::OK;
}