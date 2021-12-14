#include "events.h"

using namespace chira;

std::vector<std::string> events::broadcastsLastFrame;
std::vector<std::string> events::broadcastsThisFrame;
bool events::isRunningCallbacks = false;
std::unordered_map<std::string, std::vector<std::any>> events::calledEvents;
std::unordered_map<std::string, std::vector<std::any>> events::calledEventsFallback;
std::unordered_map<std::string, std::vector<std::pair<uuids::uuid, std::function<void(const std::any&)>>>> events::listeners;

void events::broadcast(const std::string& name) {
    if (std::count(events::broadcastsThisFrame.begin(), events::broadcastsThisFrame.end(), name) == 0)
        events::broadcastsThisFrame.push_back(name);
}

bool events::hasBroadcast(const std::string& name) {
    return std::count(events::broadcastsLastFrame.begin(), events::broadcastsLastFrame.end(), name) > 0;
}

void events::createEvent(const std::string& name, const std::any& data) {
    auto& eventMap = events::isRunningCallbacks? events::calledEventsFallback : events::calledEvents;
    if (eventMap.count(name) == 0)
        eventMap[name] = std::vector<std::any>{};
    eventMap[name].push_back(data);
}

uuids::uuid events::addListener(const std::string& name, const std::function<void(const std::any&)>& callback) {
    if (events::listeners.count(name) == 0)
        events::listeners[name] = std::vector<std::pair<uuids::uuid, std::function<void(const std::any&)>>>{};
    auto id = uuidGenerator::getNewUUID();
    events::listeners[name].push_back(std::make_pair(id, callback));
    return id;
}

bool events::removeListener(const uuids::uuid& id) {
    for (auto& [name, list] : events::listeners) {
        for (auto i = list.begin(); i < list.end(); i++) {
            if (i->first == id) {
                list.erase(i);
                return true;
            }
        }
    }
    return false;
}

void events::clearBroadcasts() {
    events::broadcastsLastFrame.clear();
    std::swap(events::broadcastsThisFrame, events::broadcastsLastFrame);
}

void events::runCallbacks() {
    events::isRunningCallbacks = true;
    while (!events::calledEvents.empty()) {
        for (const auto& [name, data] : events::calledEvents) {
            if (events::listeners.count(name) > 0) {
                for (auto& [id, callback] : events::listeners[name]) {
                    callback(data);
                }
            }
        }
        events::calledEvents.clear();
        std::swap(events::calledEvents, events::calledEventsFallback);
    }
    events::isRunningCallbacks = false;
}

void events::update() {
    events::runCallbacks();
    events::clearBroadcasts();
}
