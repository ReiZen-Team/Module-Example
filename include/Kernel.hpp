#pragma once
#include "api.hpp"
#include "ModuleManager.hpp"

namespace Kernel {
    void Init(LwClient* client);
    void LoadModulesFromEnv();
    void LoadModulesFromArgs(int argc, char** argv);
}
