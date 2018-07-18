#ifndef STORAGE_INTERFACE_H_
#define STORAGE_INTERFACE_H_

#include <string>

class StorageInterface {
 public:
 	enum Status {
 		KEY_NOT_EXIST = 0,
 		OK
 	};

 	StorageInterface();
 	virtual ~StorageInterface();

 	virtual std::string Get(const std::string& key, Status* status) = 0;
 	virtual Status Put(const std::string& key) = 0;
};

#endif