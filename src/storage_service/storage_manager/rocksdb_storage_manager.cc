#include "rocksdb_storage_manager.h"
#include "storage_service/storage_interfaces/rocksdb_storage_interface.h"

#include <memory>
#include <glog/logging.h>

RocksDbStorageManager::RocksDbStorageManager(const std::string&
    manager_hostname,
    const std::string& manager_port,
    const std::string& master_hostname,
    const std::string& master_port,
    int master_heartbeat_interval_seconds,
    const std::string& db_path): StorageManager(manager_hostname, manager_port,
	        master_hostname, master_port, master_heartbeat_interval_seconds) {
	storage_interface_.reset(new RocksDbStorageInterface(db_path));
}