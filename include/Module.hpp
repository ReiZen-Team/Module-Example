#pragma once
#include <string>
#include <vector>
#include <set>
#include <functional>
#include "api.hpp"

struct ModuleInfo {
    std::string id;
    std::string name;
    std::string version;
    std::string description;
    std::string author;
};

struct ModuleRegistrations {
    std::set<std::string> events;
    std::set<std::string> configs;
    std::set<std::string> functions;
    std::set<std::string> tools;
};

#if defined(__GNUC__) || defined(__clang__)
#define MODULE_EXPORT __attribute__((visibility("default")))
#else
#define MODULE_EXPORT
#endif

class Module {
public:
    virtual ~Module() = default;
    virtual ModuleInfo get_info() const = 0;
    virtual void on_load() {}
    virtual void on_unload() {}
    virtual std::string get_id() const { return get_info().id; }
    
    ModuleRegistrations& get_registrations() { return registrations_; }
    const ModuleRegistrations& get_registrations() const { return registrations_; }
    
protected:
    ModuleRegistrations registrations_;
};

extern "C" {
    typedef Module* (*CreateModuleFunc)();
    typedef void (*DestroyModuleFunc)(Module*);
}
