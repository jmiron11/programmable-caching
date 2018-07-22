#include "storage_service/storage_interfaces/rocksdb_storage_interface.h"
#include "storage_manager_interface.h"
#include "storage_service/storage_master/storage_master.h"
#include "storage_service/storage_manager/storage_manager.h"
#include "gtest/gtest.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>

// TODO(justinmiron): Replace client with fake and remove master.
class StorageManagerInterfaceTest : public ::testing::Test {
 public:
	StorageManagerInterfaceTest():
		master_(master_hostname, master_port),
		manager_(manager_hostname, manager_port,
		         master_hostname, master_port,
		         std::unique_ptr<StorageInterface>(new RocksDbStorageInterface("/tmp/rocksdb"))) { }

	void SetUp() override {
		master_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		manager_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		manager_interface_.reset(new StorageManagerInterface(manager_hostname,
		                         manager_port));
	}

	void TearDown() override {
		master_.Stop();
		manager_.Stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	const std::string master_hostname = "localhost";
	const std::string master_port = "50051";
	const std::string manager_hostname = "localhost";
	const std::string manager_port = "50052";
	StorageMaster master_;
	StorageManager manager_;
	std::unique_ptr<StorageManagerInterface> manager_interface_;
};


TEST_F(StorageManagerInterfaceTest, Put) {
	PutRequest request;
	LOG(INFO) << "Issuing Put Request";
	Status s = manager_interface_->Put(request);
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageManagerInterfaceTest, Get) {
	GetRequest request;
	GetReply reply;
	LOG(INFO) << "Issuing GET";
	Status s = manager_interface_->Get(request, &reply);
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageManagerInterfaceTest, Remove) {
	RemoveRequest request;
	Status s = manager_interface_->Remove(request);
	EXPECT_TRUE(s.ok());
}

TEST_F(StorageManagerInterfaceTest, CopyFrom) {
	CopyFromRequest request;
	Status s = manager_interface_->CopyFrom(request);
	EXPECT_TRUE(s.ok());
}
