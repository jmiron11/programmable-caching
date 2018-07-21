#include "storage_master.h"
#include "gflags/gflags.h"

DEFINE_string(master_hostname, "localhost", "hostname of storage master");
DEFINE_string(master_port, "50051", "port of storage master");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
	StorageMaster master(FLAGS_master_hostname, FLAGS_master_port);
  gflags::ShutDownCommandLineFlags();

  return 0;
}
