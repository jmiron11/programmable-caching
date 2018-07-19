#include "gtest/gtest.h"
#include "s3_storage_interface.h"

class S3StorageInterfaceTest : public ::testing::Test {
 public:
	S3StorageInterfaceTest(): s3_interface_(bucket_name){ }

	void SetUp() override {

	}

	void TearDown() override {

	}

	const std::string bucket_name = "testbucket";
	S3StorageInterface s3_interface_;
};


TEST_F(S3StorageInterfaceTest, TestPutGet){ }