#include <iostream>
#include <string>
#include "include/Module.hpp"
#include "include/ModuleManager.hpp"
#include "include/EventManager.hpp"
#include "include/api.hpp"

namespace ExamConfig {
    bool auto_mode_enabled = false;
    std::string test_parameter = "default_value";
    int test_counter = 0;
    bool debug_mode = false;
}

class ExamModule : public Module {
public:
    ModuleInfo get_info() const override {
        return {
            "exam_mod",
            "Exam Module",
            "1.0.0",
            "Complete test module with Events, Functions, Menu, and Config",
            "ReiZen-Team"
        };
    }

    void on_load() override {
        std::cout << "[ExamModule] Loading..." << std::endl;
        
        LwClient* client = ModuleManager::get().get_object<LwClient>("client");
        if (!client) {
            std::cerr << "[ExamModule] Error: Client not available!" << std::endl;
            return;
        }
        
        register_events(*client);
        
        register_menu_functions();
        
        register_configs();
        
        std::cout << "[ExamModule] Loaded successfully!" << std::endl;
    }

    void on_unload() override {
        std::cout << "[ExamModule] Unloading..." << std::endl;
        std::cout << "[ExamModule] Cleanup complete." << std::endl;
    }

private:
    void register_events(LwClient& client) {
        std::string module_id = get_id();
        
        if (!client.hasEvent("exam::battle_failed")) {
            client.Event("exam::battle_failed", []() {
                std::cout << "[ExamModule] Event: Battle failed handler triggered!" << std::endl;
                return json{{"status", "handled"}, {"action", "retry"}};
            }, module_id);
            registrations_.events.insert("exam::battle_failed");
        }
        
        if (!client.hasEvent("exam::login_success")) {
            client.Event("exam::login_success", []() {
                std::cout << "[ExamModule] Event: Login success handler triggered!" << std::endl;
                ExamConfig::test_counter++;
                return json{{"status", "ok"}, {"counter", ExamConfig::test_counter}};
            }, module_id);
            registrations_.events.insert("exam::login_success");
        }
        
        if (!client.hasEvent("exam::custom_test")) {
            client.Event("exam::custom_test", []() {
                std::cout << "[ExamModule] Event: Custom test event triggered!" << std::endl;
                std::cout << "[ExamModule] Auto Mode: " << (ExamConfig::auto_mode_enabled ? "ON" : "OFF") << std::endl;
                std::cout << "[ExamModule] Test Parameter: " << ExamConfig::test_parameter << std::endl;
                return json{{"status", "success"}, {"test_param", ExamConfig::test_parameter}};
            }, module_id);
            registrations_.events.insert("exam::custom_test");
        }
        
        std::cout << "[ExamModule] Registered " << registrations_.events.size() << " event handlers" << std::endl;
    }

    void register_menu_functions() {
        ModuleManager::get().register_tool("Exam: Test Function A", [](LwClient& client) {
            std::cout << "\n==================================" << std::endl;
            std::cout << "  EXAM MODULE - Test Function A  " << std::endl;
            std::cout << "==================================" << std::endl;
            std::cout << "\n[*] Executing Test Function A..." << std::endl;
            
            if (client.hasEvent("exam::custom_test")) {
                std::cout << "[*] Triggering exam::custom_test event..." << std::endl;
                client.Event("exam::custom_test");
            }
            
            std::cout << "[+] Test Function A completed!" << std::endl;
            std::cout << "[*] Counter value: " << ExamConfig::test_counter << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }, get_id());
        registrations_.tools.insert("Exam: Test Function A");
        
        ModuleManager::get().register_tool("Exam: Test Function B", [](LwClient& client) {
            std::cout << "\n==================================" << std::endl;
            std::cout << "  EXAM MODULE - Test Function B  " << std::endl;
            std::cout << "==================================" << std::endl;
            std::cout << "\n[*] Executing Test Function B..." << std::endl;
            ExamConfig::test_counter++;
            std::cout << "[+] Counter incremented to: " << ExamConfig::test_counter << std::endl;
            
            std::cout << "\n[*] Current Configuration:" << std::endl;
            std::cout << "    Auto Mode: " << (ExamConfig::auto_mode_enabled ? "Enabled" : "Disabled") << std::endl;
            std::cout << "    Test Parameter: " << ExamConfig::test_parameter << std::endl;
            std::cout << "    Test Counter: " << ExamConfig::test_counter << std::endl;
            
            std::cout << "\n[+] Test Function B completed!" << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }, get_id());
        registrations_.tools.insert("Exam: Test Function B");
        
        ModuleManager::get().register_tool("Exam: Test Function C", [](LwClient& client) {
            std::cout << "\n==================================" << std::endl;
            std::cout << "  EXAM MODULE - Test Function C  " << std::endl;
            std::cout << "==================================" << std::endl;
            std::cout << "\n[*] Interactive Test Function..." << std::endl;
            
            std::cout << "\n[?] Enter a test value: ";
            std::string input;
            std::getline(std::cin, input);
            
            if (!input.empty()) {
                ExamConfig::test_parameter = input;
                std::cout << "[+] Test parameter updated to: " << ExamConfig::test_parameter << std::endl;
            }
            
            std::cout << "\n[?] Do you want to trigger the login event? (y/n): ";
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "y" || choice == "Y") {
                if (client.hasEvent("exam::login_success")) {
                    std::cout << "[*] Triggering exam::login_success event..." << std::endl;
                    client.Event("exam::login_success");
                    std::cout << "[+] Event triggered successfully!" << std::endl;
                }
            }
            
            std::cout << "\n[+] Test Function C completed!" << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }, get_id());
        registrations_.tools.insert("Exam: Test Function C");
        
        ModuleManager::get().register_tool("Exam: Reset Module", [](LwClient& client) {
            std::cout << "\n==================================" << std::endl;
            std::cout << "    EXAM MODULE - Reset           " << std::endl;
            std::cout << "==================================" << std::endl;
            std::cout << "\n[*] Resetting Exam Module state..." << std::endl;
            
            ExamConfig::auto_mode_enabled = false;
            ExamConfig::test_parameter = "default_value";
            ExamConfig::test_counter = 0;
            
            std::cout << "[+] Module state reset to defaults!" << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::string dummy;
            std::getline(std::cin, dummy);
        }, get_id());
        registrations_.tools.insert("Exam: Reset Module");
        
        std::cout << "[ExamModule] Registered " << registrations_.tools.size() << " menu functions" << std::endl;
    }

    void register_configs() {
        ModuleManager::get().register_config_bool(
            "Exam: Auto Mode",
            []() -> bool {
                return ExamConfig::auto_mode_enabled;
            },
            [](bool value) {
                ExamConfig::auto_mode_enabled = value;
                std::cout << "[ExamModule] Auto Mode " << (value ? "enabled" : "disabled") << std::endl;
            },
            get_id()
        );
        registrations_.configs.insert("Exam: Auto Mode");
        
        ModuleManager::get().register_config_string(
            "Exam: Test Parameter",
            []() -> std::string {
                return ExamConfig::test_parameter;
            },
            [](const std::string& value) {
                ExamConfig::test_parameter = value;
                std::cout << "[ExamModule] Test Parameter set to: " << value << std::endl;
            },
            get_id()
        );
        registrations_.configs.insert("Exam: Test Parameter");
        
        ModuleManager::get().register_config_bool(
            "Exam: Debug Mode",
            []() -> bool {
                return ExamConfig::debug_mode;
            },
            [](bool value) {
                ExamConfig::debug_mode = value;
                std::cout << "[ExamModule] Debug Mode " << (value ? "enabled" : "disabled") << std::endl;
            },
            get_id()
        );
        registrations_.configs.insert("Exam: Debug Mode");
        
        std::cout << "[ExamModule] Registered " << registrations_.configs.size() << " config settings" << std::endl;
    }
};

extern "C" {
    MODULE_EXPORT Module* create_module() {
        return new ExamModule();
    }
    
    MODULE_EXPORT void destroy_module(Module* m) {
        delete m;
    }
}

#ifdef MODULE_EXAM_BUILTIN
__attribute__((constructor))
static void register_builtin_exam() {
    ModuleManager::get().register_builtin_module(std::shared_ptr<Module>(new ExamModule()));
}
#endif
