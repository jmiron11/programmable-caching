#include "gtest/gtest.h"
#include "execution_engine/execution_engine/execution_engine.h"
#include "storage_service/storage_master/storage_master.h"
#include "storage_service/storage_client/storage_client.h"
#include "scheduler/scheduler/scheduler.h" // nice.
#include "execution_engine_interface.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>

// TODO(justinmiron): Replace client with fake and remove master.
class ExecutionEngineInterfaceTest : public ::testing::Test {
 public:
	ExecutionEngineInterfaceTest():
		master_(master_hostname, master_port),
		client_(storage_client_hostname, storage_client_port,
						master_hostname, master_port),
		scheduler_(scheduler_hostname, scheduler_port,
		        	 master_hostname, master_port),
		engine_(execution_engine_hostname, execution_engine_port,
						storage_client_hostname, storage_client_port,
						scheduler_hostname, scheduler_port) { }

	void SetUp() override {
		master_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		client_.Start();
		scheduler_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		engine_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		execution_engine_interface_.reset(new ExecutionEngineInterface(execution_engine_hostname, 
		                        execution_engine_port));

	}

	void TearDown() override {
		master_.Stop();
		scheduler_.Stop();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	const std::string execution_engine_hostname = "localhost";
	const std::string execution_engine_port = "50053";
	const std::string storage_client_hostname = "localhost";
	const std::string storage_client_port = "50054";
	const std::string master_hostname = "localhost";
	const std::string master_port = "50055";
	const std::string scheduler_hostname = "localhost";
	const std::string scheduler_port = "50056";
	StorageMaster master_;
	StorageClient client_;
	Scheduler scheduler_;
	ExecutionEngine engine_;
	std::unique_ptr<ExecutionEngineInterface> execution_engine_interface_;
};

// TODO(justinmiron): Replace when fake is implemented
TEST_F(ExecutionEngineInterfaceTest, Execute) {
	Task request;
	Status s = execution_engine_interface_->Execute(request);
	EXPECT_TRUE(s.ok());
}