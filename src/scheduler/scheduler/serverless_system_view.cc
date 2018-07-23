#include "serverless_system_view.h"
#include "proto/scheduler.grpc.pb.h"
#include "proto/storage_service.grpc.pb.h"

#include <vector>
#include <string>

ServerlessSystemView::ServerlessSystemView(StorageName ephemeral, int eph_cost,
    StorageName persistent,
    int persistent_cost){ }


void ServerlessSystemView::AddExecutionEngine(const std::string& name,
    const std::string client_uri) {

}

// Changes the storage file view based off of the received GetView.
void ServerlessSystemView::ParseGetViewReply(const GetViewReply& master_view) {

}


std::vector<std::string> ServerlessSystemView::GetEphemeralStorageView(
  const std::string& engine_name) {

}

std::string ServerlessSystemView::GetEphemeralMgrFromEngine(
  const std::string& engine_name) {

}

std::string ServerlessSystemView::GetPersistentMgrForFile(
  const std::string& file_name) {

}