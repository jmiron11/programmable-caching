#ifndef ROCKSDB_STORAGE_INTERFACE_H_
#define ROCKSDB_STORAGE_INTERFACE_H_

#include "rocksdb/db.h"
#include "storage_interface.h"

// Implementation of the StorageInterface for use with RocksDb.
class RocksDbStorageInterface : public StorageInterface {
 public:
	RocksDbStorageInterface(std::string db_path);
	~RocksDbStorageInterface();

	StorageInterface::Status Get(const std::string& key,
	                             std::string* value) override;
	StorageInterface::Status Put(const std::string& key,
	                             const std::string& value) override;
	StorageInterface::Status Remove(const std::string& key) override;
	StorageInterface::Status GetAllKeys(std::vector<std::string>* keys) override;
	StorageType GetStorageType() override { return StorageType::MANAGED; }
	StorageName GetStorageName() override { return StorageName::EPHEMERAL; }
 private:
	rocksdb::DB* db_;
};

#endif