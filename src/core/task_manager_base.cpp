
#include <utility>

#include "PCServer/engine/impl/task_manager_base.hpp"

PCServer::engine::TaskManagerBase::~TaskManagerBase() = default;

PCServer::engine::TaskManagerBase::TaskManagerBase(TaskManagerConfig&& config)
    : config_(std::move(config))
{};
