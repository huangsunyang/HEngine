#include "base/EventDispatcher.hpp"

EventDispatcher * EventDispatcher::m_instance = new EventDispatcher;

void EventDispatcher::registerKeyDownEvent(int key, KeyCallback cb) {
    m_keyDownEvents[key] = cb;
}

void EventDispatcher::registerKeyUpEvent(int key, KeyCallback cb) {
    m_keyUpEvents[key] = cb;
}

void EventDispatcher::dispatchKeyDownEvent(int key) {
    if (m_keyDownEvents.find(key) != m_keyDownEvents.end()) {
        m_keyDownEvents[key]();
    }
}

void EventDispatcher::dispatchKeyUpEvent(int key) {
    if (m_keyUpEvents.find(key) != m_keyUpEvents.end()) {
        m_keyUpEvents[key]();
    }
}
