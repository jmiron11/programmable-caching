#ifndef SCHEDULER_COMMON_H_
#define SCHEDULER_COMMON_H_

#include "proto/scheduler.grpc.pb.h"

#include <string>
#include <vector>

class SchedulingDecisions {
 public:
	class Decision {
	 public:
		std::string engine;
		Task task;
		std::vector<Rule> rules;
	};

	std::vector<Decision> decisions;
};

class EngineState {
 public:
	EngineState(): name(""),
		client_name(""), ephemeral_name(""), running_tasks(0),
		maximum_tasks(-1) { }
	EngineState(std::string _name, int _maximum_tasks): name(_name),
		client_name(""), ephemeral_name(""), running_tasks(0),
		maximum_tasks(_maximum_tasks)
	{ }

	bool AvailableToSchedule() const {
		return (running_tasks < maximum_tasks &&
		        name != "" && client_name != "" && ephemeral_name != "");
	}

	std::string DebugString() const {
		return name + " " + client_name + " " + ephemeral_name;
	}

	std::string name;
	std::string client_name;
	std::string ephemeral_name;
	int running_tasks;
	int maximum_tasks;
};

class JobData {
 public:
	int total_tasks;
	int remaining_tasks;	
	std::chrono::system_clock::time_point arrival_time;
};

class TaskData {
 public:
	Task t;
};

#endif