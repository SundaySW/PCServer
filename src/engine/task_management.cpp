#include "PCServer/engine/task_management.hpp"
#include "PCServer/engine/boost_task_manager.hpp"

namespace PCServer::engine{

namespace {
    auto& DefaultTaskManagerRef() noexcept{
        static std::atomic<TaskManagerBase*> manager_ptr{&GetBoostTaskManager()};
        return manager_ptr;
    }
}

TaskManagerBase& GetTaskManager() {
    return *DefaultTaskManagerRef().load();;
}

void PushTask(TaskContext&& task) {
    GetTaskManager().ScheduleTask(std::move(task));
}

} //namespace PCServer::engine