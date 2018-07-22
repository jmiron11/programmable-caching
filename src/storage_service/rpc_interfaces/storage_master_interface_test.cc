#include "gtest/gtest.h"
#include "storage_master_interface.h"
#include "storage_service/storage_master/storage_master.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>

using grpc::Status;

class StorageMasterInterfaceTest : public ::testing::Test {
 public:
	StorageMasterInterfaceTest():
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

TEST_F(StorageMasterInterfaceTest, Introduce) {
	IntroduceRequest request;
	IntroduceReply reply;
	Status s = master_interface_->Introduce(request, &reply);
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageMasterInterfaceTest, Heartbeat) {
	Empty request, reply;
	Status s = master_interface_->Heartbeat();
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageMasterInterfaceTest, StorageChange) {
	StorageChangeRequest request;
	Status s = master_interface_->StorageChange(request);
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageMasterInterfaceTest, InstallRule) {
	InstallRuleRequest request;
	Status s = master_interface_->InstallRule(request);
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageMasterInterfaceTest, RemoveRule) {
	RemoveRuleRequest request;
	Status s = master_interface_->RemoveRule(request);
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageMasterInterfaceTest, GetView) {
	GetViewReply reply;
	Status s = master_interface_->GetView(&reply);
	EXPECT_TRUE(s.ok());
}