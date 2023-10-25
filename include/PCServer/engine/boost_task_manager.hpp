#pragma once

#include "boost/asio.hpp"

#include "moodycamel/concurrentqueue.h"

#include "PCServer/engine/impl/task_manager_base.hpp"


namespace PCServer::engine{

class BoostTaskManager: public engine::TaskManagerBase {
public:
    using Task = std::unique_ptr<TaskContext>;
    using TaskQueue = moodycamel::ConcurrentQueue<Task>;

    explicit BoostTaskManager(TaskManagerConfig);
    ~BoostTaskManager() override;
    void ScheduleTask(TaskContext&) override;
    void InitiateShutdown() override;

private:
    void Shutdown() noexcept override;
    void ProcessTasks() noexcept override;
    void PrepareThread(std::size_t index) noexcept override;

    boost::asio::thread_pool pool_;
    TaskQueue task_queue_;
};

}