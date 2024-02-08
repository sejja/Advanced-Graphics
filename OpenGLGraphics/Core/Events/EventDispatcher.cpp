//
//	EventDispatcher.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/03/23
//	Copyright © 2023. All Rights reserved
//

#include "EventDispatcher.h"

namespace Core {
    namespace Events {
        // ------------------------------------------------------------------------
        /*! Handle Event
        *
        *  EMPTY FUNCTION
        */ // --------------------------------------------------------------------
        void Listener::HandleEvent(const Event&) {}

        // ------------------------------------------------------------------------
        /*! Subscribe
        *
        *  Subscribe a Listener to an specific Event
        */ // ---------------------------------------------------------------------
        void EventDispatcher::Subscribe(Listener& who, const TypeInfo& what) {
            mEventCollection[what].insert(&who);
        }

        // ------------------------------------------------------------------------
        /*! Unsubscribe
        *
        *  Unsubscribes a Listener from a specific Event
        */ // --------------------------------------------------------------------
        void EventDispatcher::Unsubscribe(Listener& who, const TypeInfo& what) {
            auto it = mEventCollection.find(what);

            //If we found the Event
            if (it != mEventCollection.end())
                //For each Listener subscribed to the Event
                for (auto x : it->second)
                    //If the Listener is the one we want to unsubscribe
                    if (typeid(*x).name() == typeid(who).name())
                        (*it).second.erase(x);
        }

        // ------------------------------------------------------------------------
        /*! Trigger Event
        *
        *  Once an event is called, it will call every single subscribed Listener
        */ // --------------------------------------------------------------------
        void EventDispatcher::TriggerEvent(const Event& event) const {
            auto it = mEventCollection.find(event);

            //If we found the Event
            if (it != mEventCollection.end())
                //Call every subscribed object to the event
                for (auto x : it->second) x->HandleEvent(event);
        }

        std::map<TypeInfo, std::unordered_set<Listener*>> EventDispatcher::mEventCollection;
    }
}