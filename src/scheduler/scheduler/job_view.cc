#include "job_view.h"
#include "proto/scheduler.grpc.pb.h"


void JobView::AddJob(const Job& job) {

}

void JobView::RemoveTask(const Task& task) {
	
}

// Returns the pending jobs
void JobView::GetPendingTasks(std::vector<std::string>* tasks) {

}

// 
void JobView::TasksScheduled(const SchedulingDecisions& decisions) {

}


JobData JobView::GetJobData(std::string task_id) {

}

TaskData JobView::GetTaskData(std::string task_id) {
	
}