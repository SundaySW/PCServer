
#include "PCServer/engine/task_context.hpp"

namespace PCServer::engine{

//TaskContext::TaskContext(std::unique_ptr<CallT> call)
//    :call_(std::move(call))
//{
//}

TaskContext::TaskContext(CallT&& call)
        :call_(std::move(call))
{
}

TaskContext::TaskContext(TaskContext& other_context)
        :call_(std::move(other_context.call_))
{
}

void TaskContext::SetScheduledTimePoint() {
    task_scheduled_timepoint_ = std::chrono::steady_clock::now();
}

void TaskContext::SetExecuteStartedTimePoint() {
    execute_started_ = std::chrono::steady_clock::now();
}

void TaskContext::Process() {
    SetExecuteStartedTimePoint();
    call_();
//    auto call = *call_;
//    call();
    SetFinishedTimePoint();
}

void TaskContext::SetFinishedTimePoint() {
    taks_finished__timepoint_ = std::chrono::steady_clock::now();
}

std::chrono::microseconds TaskContext::GetProcessTime() {
    return std::chrono::duration_cast<std::chrono::microseconds>(taks_finished__timepoint_ - execute_started_);
}

} //namespace PCServer::engine


