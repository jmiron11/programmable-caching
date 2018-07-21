#include "storage_master.h"
#include "gtest/gtest.h"

#include <thread>
#include <chrono>

class StorageMasterTest : public ::testing::Test {
 public:
	StorageMasterTest():
		master_(hostname, port), { }

	void SetUp() override {
		master_.Start();
  	std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for startup

  	master_interface_.reset(new StorageMasterInterface(hostname, port));


	}

	void TearDown() override {
		master_.Stop();
  	std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for shutdown
	}

	const std::string hostname = "localhost";
	const std::string port = "50051";
	StorageMaster master_;
	std::unique_ptr<StorageMasterInterface> master_interface_;
	
};

TEST_F(StorageMasterTest, StartAndStop) {
}

TEST_F()