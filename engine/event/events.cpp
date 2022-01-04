#include "events.h"

using namespace chira;

std::vector<std::string> Events::broadcastsLastFrame;
std::vector<std::string> Events::broadcastsThisFrame;
bool Events::isRunningCallbacks = false;
std::unordered_map<std::string, std::vector<std::any>> Events::calledEvents;
std::unordered_map<std::string, std::vector<std::any>> Events::calledEventsFallback;
std::unordered_map<std::string, std::vector<std::pair<uuids::uuid, std::function<void(const std::any&)>>>> Events::listeners;

void Events::broadcast(const std::string& name) {
    if (std::count(Events::broadcastsThisFrame.begin(), Events::broadcastsThisFrame.end(), name) == 0)
        Events::broadcastsThisFrame.push_back(name);
}

bool Events::hasBroadcast(const std::string& name) {
    return std::count(Events::broadcastsLastFrame.begin(), Events::broadcastsLastFrame.end(), name) > 0;
}

void Events::createEvent(const std::string& name, const std::any& data) {
    auto& eventMap = Events::isRunningCallbacks ? Events::calledEventsFallback : Events::calledEvents;
    if (eventMap.count(name) == 0)
        eventMap[name] = std::vector<std::any>{};
    eventMap[name].push_back(data);
}

uuids::uuid Events::addListener(const std::string& name, const std::function<void(const std::any&)>& callback) {
    if (Events::listeners.count(name) == 0)
        Events::listeners[name] = std::vector<std::pair<uuids::uuid, std::function<void(const std::any&)>>>{};
    auto id = UUIDGenerator::getNewUUID();
    Events::listeners[name].push_back(std::make_pair(id, callback));
    return id;
}

bool Events::removeListener(const uuids::uuid& id) {
    for (auto& [name, list] : Events::listeners) {
        for (auto i = list.begin(); i < list.end(); i++) {
            if (i->first == id) {
                list.erase(i);
                return true;
            }
        }
    }
    return false;
}

void Events::clearBroadcasts() {
    Events::broadcastsLastFrame.clear();
    std::swap(Events::broadcastsThisFrame, Events::broadcastsLastFrame);
}

void Events::runCallbacks() {
    Events::isRunningCallbacks = true;
    while (!Events::calledEvents.empty()) {
        for (const auto& [name, data] : Events::calledEvents) {
            if (Events::listeners.count(name) > 0) {
                for (auto& [id, callback] : Events::listeners[name]) {
                    for (const auto& datum : data) {
                        // Yeah it's four nested loops, sue me
                        callback(datum);
                    }
                }
            }
        }
        Events::calledEvents.clear();
        std::swap(Events::calledEvents, Events::calledEventsFallback);
    }
    Events::isRunningCallbacks = false;
}

void Events::update() {
    Events::runCallbacks();
    Events::clearBroadcasts();
}
