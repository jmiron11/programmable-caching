#include "execution_engine.h"


#include "execution_engine.h"
#include "gflags/gflags.h"

DEFINE_string(execution_engine_hostname, "localhost", "hostname of execution engine");
DEFINE_string(execution_engine_port, "50051", "port of execution engine");
DEFINE_string(storage_client_hostname, "localhost", "hostname of storage client");
DEFINE_string(storage_client_port, "50052", "port of storage client");
DEFINE_string(scheduler_hostname, "localhost", "hostname of scheduler");
DEFINE_string(scheduler_port, "50052", "port of scheduler");

int main(int argc, char** argv) {
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	ExecutionEngine execution_engine(FLAGS_execution_engine_hostname,
							                     FLAGS_execution_engine_port,
							                     FLAGS_storage_client_hostname,
							                     FLAGS_storage_client_port,
							                     FLAGS_scheduler_hostname,
							                     FLAGS_scheduler_port);
	gflags::ShutDownCommandLineFlags();

	return 0;
}
