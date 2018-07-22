#ifndef ROCKSDB_STORAGE_MANAGER_H_
#define ROCKSDB_STORAGE_MANAGER_H_

#include "storage_manager.h"
#include "storage_service/storage_interfaces/rocksdb_storage_interface.h"

// Wrapper around storage manager to initialize the correct storage
// interface.
class RocksDbStorageManager : public StorageManager {
 public:
	RocksDbStorageManager(const std::string& manager_hostname,
	                      const std::string& manager_port,
	                      const std::string& master_hostname,
	                      const std::string& master_port,
	                      int master_heartbeat_interval_seconds,
	                      const std::string& db_path);
};


#endif