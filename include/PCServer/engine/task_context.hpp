#pragma once

#include <chrono>
#include <memory>
#include <functional>

namespace PCServer::engine{

    class TaskContext{
    public:
        using CallT = std::function<void()>;
        explicit TaskContext(std::unique_ptr<CallT> call);
        TaskContext(TaskContext&);
        void SetScheduledTimepoint(std::chrono::steady_clock::time_point tp);
        void SetExecuteStartedTimepoint();
        void Process();
    private:
        std::unique_ptr<CallT> call_;
        std::chrono::steady_clock::time_point task_scheduled_timepoint_;
        std::chrono::steady_clock::time_point execute_started_;
        std::chrono::steady_clock::time_point last_state_change_timepoint_;
    };

} //namespace PCServer::engine



