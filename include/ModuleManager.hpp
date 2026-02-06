#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <functional>
#include "Module.hpp"
#include "api.hpp"

struct ToolEntry {
    std::string name;
    std::function<void(LwClient&)> callback;
    std::string module_id;
};

enum class ConfigType {
    BOOL,
    STRING
};

struct ConfigEntry {
    std::string name;
    ConfigType type;
    std::string module_id;
    std::function<std::string()> getter;
    std::function<void(std::string)> setter;
};

#if defined(__GNUC__) || defined(__clang__)
#define MODULE_MANAGER_EXPORT __attribute__((visibility("default")))
#else
#define MODULE_MANAGER_EXPORT
#endif

class MODULE_MANAGER_EXPORT ModuleManager {
public:
    static ModuleManager& get();
    
    void init(LwClient* client);
    LwClient* get_client() const { return client_; }
    
    template<typename T>
    T* get_object(const std::string& name) {
        if (name == "client") {
            return reinterpret_cast<T*>(client_);
        }
        auto it = named_objects_.find(name);
        if (it != named_objects_.end()) {
            return reinterpret_cast<T*>(it->second);
        }
        return nullptr;
    }
    
    void register_object(const std::string& name, void* obj) {
        named_objects_[name] = obj;
    }

    void load_from_env(const std::string& env_var = "THLWMOD");
    void load_from_args(int argc, char** argv);
    
    bool load_module(const std::string& path);
    void unload_module(const std::string& id);
    
private:
    bool load_module_from_url(const std::string& url);
    bool load_module_from_directory(const std::string& dir_path);
    bool load_module_file(const std::string& file_path);
    std::string download_module_from_url(const std::string& url);
    
public:
    
    void register_builtin_module(std::shared_ptr<Module> module);

    void register_tool(const std::string& name, std::function<void(LwClient&)> callback, const std::string& module_id);
    
    void register_config_bool(const std::string& name,
                              std::function<bool()> getter,
                              std::function<void(bool)> setter,
                              const std::string& module_id);
    
    void register_config_string(const std::string& name,
                                std::function<std::string()> getter,
                                std::function<void(std::string)> setter,
                                const std::string& module_id);
    
    const std::map<std::string, std::shared_ptr<Module>>& get_modules() const { return modules_; }
    const std::vector<ToolEntry>& get_tools() const { return tools_; }
    const std::vector<ConfigEntry>& get_configs() const { return configs_; }
    
    bool is_builtin_module(const std::string& id) const;
    void* get_module_handle(const std::string& id) const;

private:
    ModuleManager() = default;
    
    bool check_duplicate_registrations(const Module* module) const;
    bool verify_module_signature(const std::string& path) const;
    
    LwClient* client_ = nullptr;
    std::map<std::string, std::shared_ptr<Module>> modules_;
    std::map<std::string, void*> lib_handles_;
    std::set<std::string> builtin_modules_;
    std::vector<ToolEntry> tools_;
    std::vector<ConfigEntry> configs_;
    std::map<std::string, void*> named_objects_;
};
