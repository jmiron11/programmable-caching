#include "rocksdb_storage_manager.h"
#include "storage_master.h"
#include "gtest/gtest.h"

class RocksDbStorageManagerTest : public ::testing::Test {
 public:
	RocksDbStorageManagerTest():
		master_("localhost", "50051") { }

	void SetUp() override {
		master_.Start();
	}

	void TearDown() override {
		master_.Stop();

	}

	StorageMaster master_;
};

TEST_F(RocksDbStorageManagerTest, CanAccess) {
	EXPECT_EQ(1,1);
	// RocksDbStorageManager mgr("localhost", "50051", "localhost", "50053", 5,
	//                           "/tmp/rocksdb");
	// mgr.Start();

}