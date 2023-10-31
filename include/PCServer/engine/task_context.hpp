#pragma once

#include <chrono>
#include <memory>
#include <functional>

namespace PCServer::engine{

class TaskContext{
public:
    using CallT = std::function<void()>;
//    explicit TaskContext(std::unique_ptr<CallT> call);
    explicit TaskContext(CallT&& call);
    TaskContext(TaskContext&);
    void SetScheduledTimePoint();
    void SetExecuteStartedTimePoint();
    void SetFinishedTimePoint();
    std::chrono::microseconds GetProcessTime();
    void Process();
private:
//    std::unique_ptr<CallT> call_;
    CallT call_;
    std::chrono::steady_clock::time_point task_scheduled_timepoint_;
    std::chrono::steady_clock::time_point execute_started_;
    std::chrono::steady_clock::time_point taks_finished__timepoint_;
};

} //namespace PCServer::engine



