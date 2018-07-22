#include "storage_service/storage_client/storage_client.h"
#include "storage_service/storage_master/storage_master.h"
#include "storage_service/storage_manager/storage_manager.h"
#include "storage_service/rpc_interfaces/storage_client_interface.h"
#include "storage_service/rpc_interfaces/storage_master_interface.h"
#include "storage_service/storage_interfaces/storage_interface.h"
#include "storage_service/storage_interfaces/rocksdb_storage_interface.h"
#include "gtest/gtest.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>

class StorageServiceTest : public ::testing::Test {
 public:
	StorageServiceTest():
		master_(master_hostname, master_port),
		manager1_(manager1_hostname, manager1_port,
		         master_hostname, master_port,
		         std::unique_ptr<StorageInterface>(new RocksDbStorageInterface("/tmp/rocksdb1"))),
		manager2_(manager2_hostname, manager2_port,
				         master_hostname, master_port,
				         std::unique_ptr<StorageInterface>(new RocksDbStorageInterface("/tmp/rocksdb2"))),
		client_(client_hostname, client_port, master_hostname, master_port) { 

		}

	void SetUp() override {
		master_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		manager1_.Start();
		manager2_.Start();
		client_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		master_interface_.reset(new StorageMasterInterface(master_hostname,
		                         master_port));

		client_interface_.reset(new StorageClientInterface(client_hostname,
		                        client_port));
	}

	void TearDown() override {
		master_.Stop();
		manager1_.Stop();
		manager2_.Stop();
		client_.Stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	} 

	const std::string master_hostname = "localhost";
	const std::string master_port = "50053";
	const std::string manager1_hostname = "localhost";
	const std::string manager1_port = "50055";
	const std::string manager2_hostname = "127.0.0.1";
	const std::string manager2_port = "50056";
	const std::string client_hostname = "localhost";
	const std::string client_port = "50058";
	StorageMaster master_;
	StorageManager manager1_;
	StorageManager manager2_;
	StorageClient client_;
	std::unique_ptr<StorageMasterInterface> master_interface_;
	std::unique_ptr<StorageClientInterface> client_interface_;
};

TEST_F(StorageServiceTest, StartAndStop){ }


TEST_F(StorageServiceTest, InstallPutRuleIntoManager2){ 
	// Get system view to retrieve naming of managers
	GetViewReply view_reply;
	Status s = master_interface_->GetView(&view_reply);
	EXPECT_EQ(view_reply.view_size(), 2);
	
	// Get the first of the two managers to be the chosen one.
	std::string chosen_mgr = view_reply.view(0).name();
	std::string other_mgr = view_reply.view(1).name();
	std::string client_name = "client:localhost";

	InstallRuleRequest put_rule;
	put_rule.set_client(client_name);
	Rule* rule = put_rule.mutable_rule();
	rule->set_op_type(OperationType::PUT);
	rule->set_key("key1");
	Action::PutAction* action = rule->add_action()->mutable_put_action();
	action->set_key("key1");
	action->mutable_mgr()->set_name(chosen_mgr);

	master_interface_->InstallRule(put_rule);

	PutRequest put_req;
	put_req.set_key("key1");
	put_req.set_value("value1");
	s = client_interface_->Put(put_req);
	EXPECT_TRUE(s.ok());

	// Check that the global view is updated, meaning the put worked!
	s = master_interface_->GetView(&view_reply);
	LOG(INFO) << view_reply.DebugString();
	for(const auto& view : view_reply.view()) {
		if(view.name() == chosen_mgr) {
			EXPECT_EQ(view.key_size(), 1);	
		}
	}

	// Set rule to move file from one manager to the other before retrieving it.
	InstallRuleRequest get_rule;
	get_rule.set_client(client_name);
	rule = get_rule.mutable_rule();
	rule->set_op_type(OperationType::GET);
	rule->set_key("key1");
	Action::CopyFromAction* copy_action = rule->add_action()->mutable_copy_from_action();
	copy_action->set_key("key1");
	copy_action->mutable_src_mgr()->set_name(other_mgr);
	copy_action->mutable_dst_mgr()->set_name(chosen_mgr);	
	Action::RemoveAction* remove_action = rule->add_action()->mutable_remove_action();
	remove_action->set_key("key1");
	remove_action->mutable_mgr()->set_name(chosen_mgr);
	Action::GetAction* get_action = rule->add_action()->mutable_get_action();
	get_action->set_key("key1");
	get_action->mutable_mgr()->set_name(other_mgr);

	master_interface_->InstallRule(get_rule);

	GetRequest get_req;
	GetReply get_reply;
	get_req.set_key("key1");
	s = client_interface_->Get(get_req, &get_reply);
	EXPECT_TRUE(s.ok());
	EXPECT_EQ(get_reply.value(), "value1");

	// Check that the global view is updated, meaning the put worked!
	s = master_interface_->GetView(&view_reply);
	LOG(INFO) << view_reply.DebugString();
	for(const auto& view : view_reply.view()) {
		if(view.name() == other_mgr) {
			EXPECT_EQ(view.key_size(), 1);	
		}
		else {
			EXPECT_EQ(view.key_size(), 0);
		}
	}

}