#include "gtest/gtest.h"
#include "storage_service/storage_master/storage_master.h"
#include "scheduler.h" // nice.
#include "scheduler/rpc_interfaces/scheduler_interface.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>

// TODO(justinmiron): Replace client with fake and remove master.
class SchedulerTest : public ::testing::Test {
 public:
	SchedulerTest():
		master_(master_hostname, master_port),
		scheduler_(scheduler_hostname, scheduler_port,
		        	 master_hostname, master_port, 1) { }

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

TEST_F(SchedulerTest, StartAndStop) {
	// Should wait 5s for the scheduler loop to end.
}

TEST_F(SchedulerTest, ExecutionEngineIntroduce) {
	SchedulerIntroduceRequest req;
	req.set_client_rpc_uri("hostname:1312");
	req.set_rpc_uri("hostname:1142");
	req.set_maximum_tasks(4);
	Status s = scheduler_interface_->Introduce(req);
	EXPECT_TRUE(s.ok());
}