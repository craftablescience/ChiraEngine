#include "eventQueue.h"

using namespace chira;

void eventQueue::addEvent(const std::string& eventName, const eventData& event) {
    if (eventQueue::futureEvents.count(eventName) < 1) {
        eventQueue::futureEvents[eventName] = std::vector<eventData>{};
    }
    eventQueue::futureEvents[eventName].push_back(event);
}

const std::vector<eventData>& eventQueue::hasEvent(const std::string& eventName) {
    if (eventQueue::currentEvents.count(eventName) < 1) {
        eventQueue::currentEvents[eventName] = std::vector<eventData>{};
    }
    return eventQueue::currentEvents[eventName];
}

void eventQueue::flushEvents() {
    eventQueue::currentEvents.clear();
    std::swap(eventQueue::futureEvents, eventQueue::currentEvents);
}
