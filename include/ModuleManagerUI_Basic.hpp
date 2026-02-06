#pragma once
#include <string>

class ModuleManagerUI_Basic {
public:
    static void show();
    
private:
    static void show_module_list();
    static void show_module_details(int module_index);
    static void load_module_interactive();
    static void unload_module_interactive(int module_index);
    static bool confirm_action(const std::string& message);
};
