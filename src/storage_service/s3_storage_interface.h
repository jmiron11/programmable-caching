#ifndef S3_STORAGE_INTERFACE_H_
#define S3_STORAGE_INTERFACE_H_

#include "proto/storage_service.grpc.pb.h"
#include "storage_interface.h"

// Implementation of the StorageInterface for use with AmazonS3.
class S3StorageInterface : public StorageInterface {
 public:
	S3StorageInterface(std::string db_path);
	~S3StorageInterface();

	StorageInterface::Status Get(const std::string& key,
	                						 std::string* value) override;
	StorageInterface::Status Put(const std::string& key,
	                             const std::string& value) override;
	StorageType StorageTypeIdentifier() override { return StorageType::AMAZONS3; }
 private:
 
};

#endif