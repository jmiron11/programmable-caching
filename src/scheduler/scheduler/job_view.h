#ifndef JOB_VIEW_H_
#define JOB_VIEW_H_

#include "scheduler_common.h"
#include "proto/scheduler.grpc.pb.h"

#include <vector>
#include <string>

class JobView {
 public:

 	void AddJob(const Job& job);

 	void RemoveTask(const Task& task);

 	// Returns the pending jobs
 	void GetPendingTasks(std::vector<std::string>* tasks);

 	//
 	void TasksScheduled(const SchedulingDecisions& decisions);


 	JobData GetJobData(std::string task_id);

 	TaskData GetTaskData(std::string task_id);


 	// Maintain a giant list of pending tasks.
 	// For each task, map it to a JobData and TaskData.


};

#endif