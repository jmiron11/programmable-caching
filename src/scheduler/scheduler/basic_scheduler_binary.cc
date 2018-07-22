#include "scheduler.h"
#include "gflags/gflags.h"

DEFINE_string(scheduler_hostname, "localhost", "hostname of scheduler");
DEFINE_string(scheduler_port, "50051", "port of scheduler");
DEFINE_string(master_hostname, "localhost", "hostname of storage service");
DEFINE_string(master_port, "50052", "port of storage service");

int main(int argc, char** argv) {
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	Scheduler client(FLAGS_scheduler_hostname,
	                     FLAGS_scheduler_port,
	                     FLAGS_master_hostname,
	                     FLAGS_master_port);
	gflags::ShutDownCommandLineFlags();

	return 0;
}
