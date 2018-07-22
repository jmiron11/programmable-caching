#ifndef REDIS_STORAGE_INTERFACE_H_
#define REDIS_STORAGE_INTERFACE_H_

#include "storage_interface.h"

#include "hiredis/hiredis.h"

class RedisInterface : public StorageInterface {
 public:
	RedisInterface(std::string hostname, std::string port);
	~RedisInterface();

	StorageInterface::Status Get(const std::string& key,
	                             std::string* value) override;
	StorageInterface::Status Put(const std::string& key,
	                             const std::string& value) override;
	StorageInterface::Status Remove(const std::string& key) override;
	StorageInterface::Status GetAllKeys(std::vector<std::string>* keys) override;
	StorageType GetStorageType() override { return StorageType::MANAGED; }
	StorageName GetStorageName() override { return StorageName::EPHEMERAL; }
 private:
 	redisContext * context_;	
};


#endif