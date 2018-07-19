
#include "storage_interface.h"
#include "rocksdb_storage_interface.h"
#include "rocksdb/db.h"

#include <glog/logging.h>

RocksDbStorageInterface::RocksDbStorageInterface(std::string db_path) {
	rocksdb::Options options;
	options.create_if_missing = true;
	rocksdb::Status s = rocksdb::DB::Open(options, db_path, &db_);
	if (!s.ok()) {
		LOG(ERROR) << s.ToString();

	}
	assert(s.ok());
}

RocksDbStorageInterface::~RocksDbStorageInterface() {
	// Clean up rocksdb data structures
	delete db_;
}

StorageInterface::Status RocksDbStorageInterface::Get(const std::string& key,
		std::string* value) {	
	rocksdb::Status s = db_->Get(rocksdb::ReadOptions(), key, value);
	if(!s.ok()) {
		LOG(ERROR) << s.ToString();
		return StorageInterface::GENERIC_ERROR;
	}
	else {
		return StorageInterface::OK;
	}
}

StorageInterface::Status RocksDbStorageInterface::Put(const std::string& key,
    const std::string& value) {
	rocksdb::Status s = db_->Put(rocksdb::WriteOptions(), key, value);
	if(!s.ok()) {
		LOG(ERROR) << s.ToString();
		return StorageInterface::GENERIC_ERROR;
	}
	else {
		return StorageInterface::OK;
	}
}

