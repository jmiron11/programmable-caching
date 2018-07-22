#include "gtest/gtest.h"
#include "execution_engine/execution_engine/execution_engine.h"
#include "storage_service/storage_interfaces/rocksdb_storage_interface.h"
#include "storage_service/storage_master/storage_master.h"
#include "storage_service/storage_manager/storage_manager.h"
#include "storage_service/storage_client/storage_client.h"
#include "scheduler/scheduler/scheduler.h" // nice.
#include "scheduler/rpc_interfaces/scheduler_interface.h"

#include <grpcpp/grpcpp.h>
#include <thread>
#include <chrono>

class MaestroTest : public ::testing::Test {
 public:
	MaestroTest():
		master_(master_hostname, master_port),
		client_(storage_client_hostname, storage_client_port,
						master_hostname, master_port),
		manager_(manager_hostname, manager_port,
						 master_hostname, master_port,
						 std::unique_ptr<StorageInterface>(new RocksDbStorageInterface("/tmp/rocksdb"))),
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
		manager_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		engine_.Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		scheduler_interface_.reset(new SchedulerInterface(scheduler_hostname, 
		                        scheduler_port));

	}

	void TearDown() override {
		master_.Stop();
		scheduler_.Stop();
		manager_.Stop();
		engine_.Stop();
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
	const std::string manager_hostname = "localhost";
	const std::string manager_port = "50057";

	StorageMaster master_;
	StorageClient client_;
	StorageManager manager_;
	Scheduler scheduler_;
	ExecutionEngine engine_;

	std::unique_ptr<SchedulerInterface> scheduler_interface_;
};

TEST_F(MaestroTest, InitializationAndTeardown) { }