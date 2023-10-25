
#include "PCServer/engine/boost_task_manager.hpp"

#include "PCServer/logging/log.hpp"

namespace PCServer::engine{

using namespace PCServer::logging;

BoostTaskManager::BoostTaskManager(TaskManagerConfig config)
    :TaskManagerBase(std::move(config)),
    pool_(config.worker_threads)
{
    try {
        LOG_INFO() << "creating task_manager " << Name() << " "
                   << "worker_threads=" << config_.worker_threads;
        for (size_t i = 0; i < config_.worker_threads; ++i) {
            boost::asio::post(pool_, [this]{
                ProcessTasks();
            });
        }
    } catch (...) {
        Shutdown();
        throw;
    }
}

void BoostTaskManager::ScheduleTask(TaskContext& context) {
    task_queue_.enqueue(std::make_unique<TaskContext>(context));
}

void BoostTaskManager::InitiateShutdown() {

}

void BoostTaskManager::Shutdown() noexcept {
    pool_.join();
}

void BoostTaskManager::ProcessTasks() noexcept {
    while (true) {
        Task task;
        auto hasElem = task_queue_.try_dequeue(task);
        if(!hasElem)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        task->SetExecuteStartedTimepoint();

        bool processed_ok = true;
        try {
            task->Process();
        } catch (const std::exception& ex){
            LOG_ERROR() << "uncaught exception from ProcessTask : " << ex;
            processed_ok = false;
        }

        if(processed_ok) {
           //todo finish task
        }
    }
}

void BoostTaskManager::PrepareThread(std::size_t index) noexcept {

}

BoostTaskManager::~BoostTaskManager() {

}

} //namespace PCServer::engine


