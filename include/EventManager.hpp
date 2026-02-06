#pragma once
#include <string>
#include "api.hpp"

class EventManager {
public:
    static void show(LwClient& client);
    
private:
    static void display_event_details(LwClient& client, const std::string& event_name);
};
