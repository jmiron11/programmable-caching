#ifndef STORAGE_INTERFACE_H_
#define STORAGE_INTERFACE_H_

#include "proto/storage_service.grpc.pb.h"

#include <string>

class StorageInterface {
 public:
 	enum Status {
 		GENERIC_ERROR = 0,
 		OK
 	};

 	StorageInterface(){ }
 	virtual ~StorageInterface(){ }

 	// Retrieve value associated with key from storage medium and store in
 	// value parameter.
 	virtual Status Get(const std::string& key, std::string* value) = 0;

 	// Put key,value pair into storage medium.
 	virtual Status Put(const std::string& key, const std::string& value) = 0;

 	// Return identification string of storage medium
 	virtual StorageType StorageTypeIdentifier() = 0;
};

#endif