#pragma once

#include <string>

namespace PCServer::engine {

    struct TaskManagerConfig {
        std::string name;
        bool should_guess_cpu_limit{false};
        std::size_t worker_threads{4};
        int spinning_iterations{10000};
        std::size_t task_trace_every{1000};
    };

} //namespace PCServer::engine