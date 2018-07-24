#include "scheduler.h"
#include "execution_engine/rpc_interfaces/execution_engine_interface.h"

#include <grpcpp/grpcpp.h>
#include <glog/logging.h>

#include <string>
#include <chrono>

using grpc::Status;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using std::string;

namespace {
bool compare(const std::pair<std::string, int>&i,
             const std::pair<std::string, int>&j) {
	return i.second < j.second;
}
}

Scheduler::Scheduler(const std::string& scheduler_hostname,
                     const std::string& scheduler_port,
                     const std::string& storage_hostname,
                     const std::string& storage_port,
                     int scheduler_interval_seconds):
	system_view_(StorageName::EPHEMERAL, StorageName::FAKE_PERSISTANT),
	storage_interface_(storage_hostname, storage_port),
	scheduler_hostname_(scheduler_hostname),
	scheduler_port_(scheduler_port),
	running_(false),
	scheduler_interval_seconds_(scheduler_interval_seconds) {
}

void Scheduler::Start() {
	// Begin processing of storage management RPCs.
	std::string server_address(scheduler_hostname_ + ':' + scheduler_port_);
	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(this);
	server_ = builder.BuildAndStart();
	LOG(INFO) << "Scheduler listening on " << server_address;


	// Begin scheduling jobs
	scheduler_mutex_.lock();
	running_ = true;
	scheduler_mutex_.unlock();
	scheduler_thread_ = std::thread(&Scheduler::SchedulerControlThread, this);
}

void Scheduler::Stop() {
	scheduler_mutex_.lock();
	running_ = false;
	scheduler_mutex_.unlock();
	scheduler_thread_.join();

	server_->Shutdown();
	LOG(INFO) << "Scheduler shutdown";
}

Status Scheduler::SubmitJob(ServerContext* context,
                            const Job* request,
                            Empty* reply) {

	// Add job to the system state
	job_view_.AddJob(*request);
	return Status::OK;
}

Status Scheduler::Done(ServerContext* context,
                       const Task* request,
                       Empty* reply) {

	// Remove task from the system state
	job_view_.RemoveTask(*request);
	return Status::OK;
}

Status Scheduler::Introduce(ServerContext* context,
                            const SchedulerIntroduceRequest* request,
                            Empty* reply) {
	// Named after rpc_uri.
	system_view_.AddExecutionEngine(request->rpc_uri(), request->client_rpc_uri(),
	                                request->maximum_tasks());

	return Status::OK;
}

void Scheduler::Schedule(std::vector<std::string> pending_tasks,
                         SchedulingDecisions* decisions) {
	// Performs SJF and random server, retrieving remotely.

	// Create vector of: task -> job remaining size pair
	std::vector<std::pair<std::string, int>> task_size_pairs;

	for (const auto& task : pending_tasks) {
		JobData j = job_view_.GetJobData(task);
		task_size_pairs.push_back(std::pair<std::string, int>(task, j.remaining_tasks));
	}

	// Sort vector by the remaining number of elements.
	std::sort(task_size_pairs.begin(), task_size_pairs.end(),
	          compare);

	// Iterate over the server state, for each available server spot, pick the
	// next task in the sorted list.
	uint task_idx = 0;
	std::vector<std::string> schedulable_engines;
	system_view_.GetSchedulableEngines(&schedulable_engines);
	for (const auto& engine : schedulable_engines) {
		EngineState s = system_view_.GetEngineState(engine);
		int available = s.maximum_tasks - s.running_tasks;

		while (available > 0) {
			// If there are no remaining tasks
			if (task_idx >= task_size_pairs.size()) {
				break;
			}

			decisions->decisions.emplace_back();
			auto d = decisions->decisions.back();
			d.task = job_view_.GetTaskData(task_size_pairs[task_idx].first).t;

			std::vector<Rule> & r = d.rules;
			if (d.task.input_file() != "") {
				r.emplace_back();
				Rule& r_in = r.back();
				// TODO(justinmiron): Put into helper function.
				r_in.set_op_type(OperationType::GET);
				r_in.set_key(d.task.input_file());
				Action::GetAction * act = r_in.add_action()->mutable_get_action();
				act->set_key(d.task.input_file());
				act->mutable_mgr()->set_name(system_view_.GetPersistentMgrForFile(
				                               d.task.input_file()));
			}

			if (d.task.output_file() != "") {
				r.emplace_back();
				Rule& r_in = r.back();
				// TODO(justinmiron): Put into helper function.
				r_in.set_op_type(OperationType::PUT);
				r_in.set_key(d.task.output_file());
				Action::PutAction * act = r_in.add_action()->mutable_put_action();
				act->set_key(d.task.output_file());
				act->mutable_mgr()->set_name(system_view_.GetPersistentMgrForPut(
				                               d.task.output_file()));
			}

			d.engine = engine;

			available--;
			task_idx++;
		}
	}
}

void Scheduler::Execute(const SchedulingDecisions& decisions) {
	// For each of the scheduling decisions:

	// TODO(justinmiron): parallelize.
	for (const auto& decision : decisions.decisions) {
		EngineState s = system_view_.GetEngineState(decision.engine);

		// RPC to master to install rules

		// TODO(justinmiron): Batch rules together, since they are all to same
		// client.
		for (const auto& rule : decision.rules) {
			InstallRuleRequest req;
			*req.mutable_rule() = rule;
			req.set_client(s.client_name);
			storage_interface_.InstallRule(req);
		}

		// Connect to engine (engine_name is the uri).
		ExecutionEngineInterface engine(decision.engine);

		// Send the task
		engine.Execute(decision.task);
	}
}


void Scheduler::SchedulerControlThread() {
	scheduler_mutex_.lock();
	bool running = running_;
	scheduler_mutex_.unlock();

	while (running) {
		std::this_thread::sleep_for(std::chrono::seconds(scheduler_interval_seconds_));

		// Get global view of the system
		GetViewReply view_reply;
		storage_interface_.GetView(&view_reply);

		// // Snapshot the system state.
		// ServerlessSystemView system_snapshot = system_view_;

		// Call schedule function.
		std::vector<std::string> pending_jobs;
		job_view_.GetPendingTasks(&pending_jobs);

		if (pending_jobs.size() > 0) {
			SchedulingDecisions decisions;
			Schedule(pending_jobs, &decisions);

			// Run execution of scheduling decisions.
			job_view_.TasksScheduled(decisions);
			system_view_.TasksScheduled(decisions);
			Execute(decisions);
		}

		scheduler_mutex_.lock();
		running = running_;
		scheduler_mutex_.unlock();
	}
}