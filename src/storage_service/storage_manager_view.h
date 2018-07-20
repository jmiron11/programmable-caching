#ifndef STORAGE_MANAGER_VIEW_H_
#define STORAGE_MANAGER_VIEW_H_

#include "proto/storage_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>
#include <string>
#include <unordered_map>
#include <vector>

// Maintains state on storage managers
class StorageManagerView {
 public:

	class View {
	 public:
	 	View(){}
		View(const std::string& name, const std::string& uri);
		StorageManagerService::Stub* GetStub();
		std::string GetName();
		std::string GetUri();
	 public:
		std::unique_ptr<StorageManagerService::Stub> stub_;
		std::string uri_;
		std::string name_;
	};

	void Add(const std::string& name, const std::string& uri);
	std::vector<StorageManagerService::Stub*> GetAllStubs();
	std::vector<std::pair<std::string, std::string>> GetAllInfo();

 private:
	std::unordered_map<std::string, View> manager_view_;
	std::mutex shared_mutex;
};



#endif