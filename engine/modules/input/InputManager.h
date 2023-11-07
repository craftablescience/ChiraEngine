#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <SDL_keycode.h>

namespace chira::Input {

template<typename T, typename U, typename... CallbackArgs>
class Event {
public:
    Event(T event_, U eventType_, std::function<void(CallbackArgs...)> func_)
        : event(event_)
        , eventType(eventType_)
        , func(std::move(func_)) {}
    [[nodiscard]] inline T getEvent() const {
        return this->event;
    }
    [[nodiscard]] inline U getEventType() const {
        return this->eventType;
    }
    inline void operator()(CallbackArgs... args) const {
        this->func(args...);
    }

    static inline void create(T event, U eventType, std::function<void(CallbackArgs...)> func) {
        Event<T, U, CallbackArgs...>::events.emplace_back(event, eventType, std::move(func));
    }

    static inline const std::vector<Event<T, U, CallbackArgs...>>& getEvents() {
        return Event<T, U, CallbackArgs...>::events;
    }
protected:
    T event;
    U eventType;
    std::function<void(CallbackArgs...)> func;

    static inline std::vector<Event<T, U, CallbackArgs...>> events;
};

using Key = SDL_KeyCode;
enum class KeyEventType {
    RELEASED,
    PRESSED,
    REPEATED,
};
using KeyEvent = Event<Key, KeyEventType>;

// Match SDL_BUTTON_<X>
enum class Mouse : uint8_t {
    BUTTON_LEFT    = 1,
    BUTTON_MIDDLE  = 2,
    BUTTON_RIGHT   = 3,
    BUTTON_X1      = 4,
    BUTTON_X2      = 5,
};
enum class MouseEventType {
    RELEASED,
    CLICKED,
};
using MouseEvent = Event<Mouse, MouseEventType, int, int, uint8_t>;

enum class MouseMotion {
    MOVEMENT,
    SCROLL,
};
enum class MouseMotionEventType {
    NOT_APPLICABLE,
};
using MouseMotionEvent = Event<MouseMotion, MouseMotionEventType, int, int, int, int>;

} // namespace chira::Input
