#pragma once
#include <map>
#include <vector>
#include <functional>

using std::map;
using std::vector;

class EventDispatcher {
public:
    using KeyCallback = std::function<void(void)>;

    static EventDispatcher * instance() {
        return m_instance;
    }

    void registerKeyDownEvent(int, KeyCallback);
    void registerKeyUpEvent(int, KeyCallback);
    void dispatchKeyDownEvent(int);
    void dispatchKeyUpEvent(int);

protected:
    EventDispatcher(){}

    static EventDispatcher * m_instance;

    map<int, KeyCallback> m_keyDownEvents;
    map<int, KeyCallback> m_keyUpEvents;
};