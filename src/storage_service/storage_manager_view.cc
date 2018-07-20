#include "storage_manager_view.h"
#include "proto/storage_service.grpc.pb.h"

#include <string>
#include <grpcpp/grpcpp.h>

StorageManagerView::View::View(const std::string& name,
                               const std::string& uri) {
	stub_ = StorageManagerService::NewStub(grpc::CreateChannel(
	    uri,
	    grpc::InsecureChannelCredentials()));
	name_ = name;
	uri_ = uri;
}

StorageManagerService::Stub* StorageManagerView::View::GetStub() {
	return stub_.get();
}

std::string StorageManagerView::View::GetName() {
	return name_;
}

std::string StorageManagerView::View::GetUri() {
	return uri_;
}

void StorageManagerView::Add(const std::string& name,
                             const std::string& uri) {
	std::lock_guard<std::mutex> guard(shared_mutex);
	manager_view_[name] = View(name, uri);
}

std::vector<StorageManagerService::Stub*> StorageManagerView::GetAllStubs() {
	std::lock_guard<std::mutex> guard(shared_mutex);
	std::vector<StorageManagerService::Stub*> stubs;

	for (auto& view : manager_view_) {
		stubs.push_back(view.second.GetStub());
	}

	return stubs;
}

std::vector<std::pair<std::string, std::string>>
StorageManagerView::GetAllInfo() {
	std::lock_guard<std::mutex> guard(shared_mutex);
	std::vector<std::pair<std::string, std::string>> data;

	for (auto& view : manager_view_) {
		data.push_back(std::pair<std::string, std::string>(view.second.GetName(),
		               view.second.GetUri()));
	}

	return data;
}
