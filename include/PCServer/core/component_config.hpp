#pragma once

#include "nlohmann/json.hpp"

class ComponentConfig final{
public:
    explicit ComponentConfig(std::string name)
        :name_(std::move(name))
    {};
private:
    std::string name_;
    nlohmann::json config;
};