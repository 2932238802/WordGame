#pragma once


#include <functional>
#include <regex>
#include <vector>
#include "common/constant.h"

namespace LosG{
    using func_t = std::function<void(const std::smatch&)>;
    struct Command{
        std::regex reg;
        func_t handle;
        std::string help;
    };
};