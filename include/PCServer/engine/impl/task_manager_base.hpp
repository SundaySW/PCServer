#pragma once

#include "PCServer/engine/task_manager_config.hpp"
#include "PCServer/engine/task_context.hpp"

namespace PCServer::engine{

class TaskManagerBase {
public:
    TaskManagerBase(TaskManagerConfig);
    virtual ~TaskManagerBase();
    virtual void ScheduleTask(TaskContext&&) = 0;
    virtual void InitiateShutdown() = 0;
    const std::string& Name() {return config_.name;}
protected:
    const TaskManagerConfig config_;
private:
    virtual void Shutdown() noexcept = 0;
    virtual void ProcessTasks() noexcept = 0;
    virtual void PrepareThread(std::size_t index) noexcept = 0;
};

} //namespace PCServer::engine



