#include "execution_engine.h"


ExecutionEngine::ExecutionEngine(const std::string& execution_engine_hostname,
                const std::string& execution_engine_port,
                const std::string& storage_client_hostname,
                const std::string& storage_client_port,
                const std::string& scheduler_hostname,
                const std::string& scheduler_port) {

}

void ExecutionEngine::Start() {

}

void ExecutionEngine::Stop() {

}

Status ExecutionEngine::Execute(ServerContext* context,
                 const Task* request,
                 Empty* reply){ 

}
