#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <any>
#include <utility/UUIDGenerator.h>

namespace chira {

class Events {
public:
    /// Visible on the next frame
    static void broadcast(const std::string& name);
    static bool hasBroadcast(const std::string& name);
    /// Event is handled after all entities execute in current frame
    static void createEvent(const std::string& name, const std::any& data = nullptr);
    static uuids::uuid addListener(const std::string& name, const std::function<void(const std::any&)>& callback);
    static bool removeListener(const uuids::uuid& id);
    static void clearBroadcasts();
    static void runCallbacks();
    static void update();
private:
    static inline std::vector<std::string> broadcastsLastFrame;
    static inline std::vector<std::string> broadcastsThisFrame;
    static inline bool isRunningCallbacks = false;
    static inline std::unordered_map<std::string, std::vector<std::any>> calledEvents;
    static inline std::unordered_map<std::string, std::vector<std::any>> calledEventsFallback;
    static inline std::unordered_map<std::string, std::vector<std::pair<uuids::uuid, std::function<void(const std::any&)>>>> listeners;
};

} // namespace chira
