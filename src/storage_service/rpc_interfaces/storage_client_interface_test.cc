#include "gtest/gtest.h"
#include "storage_service/storage_master/storage_master.h"

#include "storage_client_interface.h"
#include "storage_service/storage_client/storage_client.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>

// TODO(justinmiron): Replace client with fake and remove master.
class StorageClientInterfaceTest : public ::testing::Test {
 public:
	StorageClientInterfaceTest():
		master_(master_hostname, master_port),
		client_(client_hostname, client_port,
		        master_hostname, master_port) { }

	void SetUp() override {
		master_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		client_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		client_interface_.reset(new StorageClientInterface(client_hostname, 
		                        client_port));

	}

	void TearDown() override {
		master_.Stop();
		client_.Stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	const std::string master_hostname = "localhost";
	const std::string master_port = "50053";
	const std::string client_hostname = "localhost";
	const std::string client_port = "50055";
	StorageMaster master_;
	StorageClient client_;
	std::unique_ptr<StorageClientInterface> client_interface_;
};

// TODO(justinmiron): Replace when fake is implemented
// TEST_F(StorageClientInterfaceTest, Put) {
// 	PutRequest request;
// 	Status s = client_interface_->Put(request);
// 	EXPECT_TRUE(s.ok());
// }

// TEST_F(StorageClientInterfaceTest, Get) {
// 	GetRequest request;
// 	GetReply reply;
// 	Status s = client_interface_->Get(request, &reply);
// 	EXPECT_TRUE(s.ok());
// }

// TEST_F(StorageClientInterfaceTest, Remove) {
// 	RemoveRequest request;
// 	Status s = client_interface_->Remove(request);
// 	EXPECT_TRUE(s.ok());
// }

TEST_F(StorageClientInterfaceTest, InstallRule) {
	InstallRuleRequest request;
	Status s = client_interface_->InstallRule(request);
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageClientInterfaceTest, RemoveRule) {
	RemoveRuleRequest request;
	Status s = client_interface_->RemoveRule(request);
	EXPECT_TRUE(s.ok());
}