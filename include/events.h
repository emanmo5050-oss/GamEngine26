#pragma once
#include <vector>
#include <string>

using namespace std;

// Task 2 - Amy
// Event System using Observer Pattern

// Base Event class
class Event {
public:
    string type;
    Event(string t) : type(t) {}
    virtual ~Event() {}
};

// Player Events
class PlayerJoinedEvent : public Event {
public:
    string username;
    PlayerJoinedEvent(string u) : Event("PlayerJoined"), username(u) {}
};

class PlayerLeftEvent : public Event {
public:
    string username;
    PlayerLeftEvent(string u) : Event("PlayerLeft"), username(u) {}
};

// Collision Event
class CollisionEvent : public Event {
public:
   // Entity* a;
   // Entity* b;
  //  CollisionEvent(Entity* a, Entity* b) : Event("Collision"), a(a), b(b) {}  // Note: waiting for Yassin to finish Entity class
};

// Score Event
class ScoreUpdateEvent : public Event {
public:
    int playerID;
    int newScore;
    ScoreUpdateEvent(int id, int s) : Event("ScoreUpdate"), playerID(id), newScore(s) {}
};

// Listener interface
class EventListener {
public:
    virtual void onEvent(Event* e) = 0;
};

// EventManager
class EventManager {
private:
    vector<EventListener*> listeners;
public:
    void subscribe(EventListener* l);
    void notify(Event* e);
};