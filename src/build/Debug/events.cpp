#include "events.h"

// Task 2 - Amy
// EventManager implementation

void EventManager::subscribe(EventListener* l) {
    listeners.push_back(l);
}

void EventManager::notify(Event* e) {
    for (auto l : listeners) {
        l->onEvent(e);
    }
}