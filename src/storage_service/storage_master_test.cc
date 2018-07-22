#include "storage_master.h"
#include "storage_master_interface.h"
#include "gtest/gtest.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>
#include <set>

using grpc::Status;

class StorageMasterTest : public ::testing::Test {
 public:
	StorageMasterTest():
		master_(hostname, port) { }

	void SetUp() override {
		master_.Start();

		// Delay for startup
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		master_interface_.reset(new StorageMasterInterface(hostname, port));
	}

	void TearDown() override {
		master_.Stop();

		// Delay for shutdown
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	const std::string hostname = "localhost";
	const std::string port = "50051";
	StorageMaster master_;
	std::unique_ptr<StorageMasterInterface> master_interface_;
};

TEST_F(StorageMasterTest, StartAndStop) {
}

//
TEST_F(StorageMasterTest, UpdateToDateGlobalView) {

	// Introduce RPC from storage manager with files
	IntroduceRequest request;
	IntroduceRequest::StorageManagerIntroduce* introduce =
	  request.mutable_storage_manager();
	introduce->set_storage_type(StorageType::MANAGED);
	introduce->set_storage_name(StorageName::EPHEMERAL);
	introduce->set_rpc_hostname("hostname");
	introduce->set_rpc_port("1234");

	std::set<std::string> manager_files = {"f1", "f2"}; 
	for (const auto& f : manager_files) {
		IntroduceRequest::StorageManagerIntroduce::FileId* id = introduce->add_file();
		id->set_key(f);
	}

	IntroduceReply reply;
	Status s = master_interface_->Introduce(request, &reply);
	EXPECT_TRUE(s.ok());

	// Retrieve the manager name for later delta
	std::string mgr_name = reply.name();


	// Check that the view is equal to the initial set of manager files in 
	// introduce.
	GetViewReply view_reply;
	master_interface_->GetView(&view_reply);
	EXPECT_EQ(view_reply.view_size(), 1);	

	const GetViewReply::ManagerView& mgr_view = view_reply.view(0);
	EXPECT_EQ(mgr_view.name(), mgr_name);
	EXPECT_EQ(manager_files.size(), mgr_view.key_size());

	// Each manager file actually exists in the view.
	for(const auto& key : mgr_view.key()){
		EXPECT_NE(manager_files.find(key), manager_files.end());
	}

	// Remove f2 from mgr and add f3.
	StorageChangeRequest change_request;
	StorageChangeRequest::Delta* d;
	d = change_request.add_storage_change(); 

	d->set_op(OperationType::PUT);
	d->set_key("f3");
	d->set_manager(mgr_name);

	d = change_request.add_storage_change();
	d->set_op(OperationType::REMOVE);
	d->set_key("f2");
	d->set_manager(mgr_name);

	// Issue storage change to the master
	master_interface_->StorageChange(change_request);

	std::set<std::string> new_set_of_files = {"f1", "f3"};
	GetViewReply new_view_reply;
	master_interface_->GetView(&new_view_reply);
	EXPECT_EQ(new_view_reply.view_size(), 1);	

	const GetViewReply::ManagerView& new_mgr_view = new_view_reply.view(0);
	EXPECT_EQ(new_mgr_view.name(), mgr_name);
	EXPECT_EQ(new_set_of_files.size(), new_mgr_view.key_size());

	// Each manager file actually exists in the view.
	LOG(INFO) << "new keys";
	for(const auto& key : new_mgr_view.key()){
		LOG(INFO) << key;
		EXPECT_NE(new_set_of_files.find(key), new_set_of_files.end());
	}
}