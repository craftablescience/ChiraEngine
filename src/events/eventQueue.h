#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace chira {
    /// The "name" variable should be set to the sender's name.
    /// "data" is the data to send, if any.
    struct eventData {
        std::string sender;
        std::string data;
    };

    /// Stores a list of fired events generated as the game progresses.
    /// Fired events of this frame are visible the next frame.
    /// When the frame is done and everything has processed,
    /// flush currentEvents and move the contents of futureEvents to it.
    /// Think of it like v-sync's double buffer.
    class eventQueue {
        friend class engine; // to call flushEvents()
    public:
        static void addEvent(const std::string& eventName, const eventData& event);
        /// Note: the returned array is not guaranteed to exist after
        /// the flushEvents() function executes at the end of the frame!
        static const std::vector<eventData>& hasEvent(const std::string& eventName);
    private:
        static inline std::unordered_map<std::string, std::vector<eventData>> currentEvents{};
        static inline std::unordered_map<std::string, std::vector<eventData>> futureEvents{};
        /// Called by the engine at the end of the frame
        static void flushEvents();
    };
}
