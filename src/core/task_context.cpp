
#include "PCServer/engine/task_context.hpp"

namespace PCServer::engine{

TaskContext::TaskContext(std::unique_ptr<CallT> call)
    :call_(std::move(call))
{
}

TaskContext::TaskContext(TaskContext& other_context)
        :call_(std::move(other_context.call_))
{
}

void TaskContext::SetScheduledTimepoint(std::chrono::steady_clock::time_point tp) {
    task_scheduled_timepoint_ = tp;
}

void TaskContext::SetExecuteStartedTimepoint() {
    execute_started_ = std::chrono::steady_clock::now();
}

void TaskContext::Process() {
    auto call = *call_;
    call();
}

} //namespace PCServer::engine


