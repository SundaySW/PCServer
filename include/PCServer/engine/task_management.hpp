#pragma once

#include "PCServer/engine/impl/task_manager_base.hpp"
#include "PCServer/engine/task_context.hpp"

namespace PCServer::engine{

TaskManagerBase& GetTaskManager();

void PushTask(TaskContext&& task);

} //namespace PCServer::engine
