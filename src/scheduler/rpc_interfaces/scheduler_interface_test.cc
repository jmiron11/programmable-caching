#include "gtest/gtest.h"
#include "storage_service/storage_master/storage_master.h"
#include "scheduler/scheduler/scheduler.h" // nice.
#include "scheduler_interface.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>

// TODO(justinmiron): Replace client with fake and remove master.
class SchedulerInterfaceTest : public ::testing::Test {
 public:
	SchedulerInterfaceTest():
		master_(master_hostname, master_port),
		scheduler_(scheduler_hostname, scheduler_port,
		        	 master_hostname, master_port) { }

	void SetUp() override {
		master_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		scheduler_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		scheduler_interface_.reset(new SchedulerInterface(scheduler_hostname, 
		                        scheduler_port));

	}

	void TearDown() override {
		master_.Stop();
		scheduler_.Stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	const std::string master_hostname = "localhost";
	const std::string master_port = "50053";
	const std::string scheduler_hostname = "localhost";
	const std::string scheduler_port = "50055";
	StorageMaster master_;
	Scheduler scheduler_;
	std::unique_ptr<SchedulerInterface> scheduler_interface_;
};

// TODO(justinmiron): Replace when fake is implemented
TEST_F(SchedulerInterfaceTest, SubmitJob) {
	Job request;
	Status s = scheduler_interface_->SubmitJob(request);
	EXPECT_TRUE(s.ok());
}

TEST_F(SchedulerInterfaceTest, Done) {
	Task request;
	Status s = scheduler_interface_->Done(request);
	EXPECT_TRUE(s.ok());
}

TEST_F(SchedulerInterfaceTest, Introduce) {
	SchedulerIntroduceRequest request;
	Status s = scheduler_interface_->Introduce(request);
	EXPECT_TRUE(s.ok());
}

