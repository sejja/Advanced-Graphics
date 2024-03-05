//
//	EventDispatcher.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 05/03/24
//	Copyright © 2024. All Rights reserved
//

#include "EventDispatcher.h"
#include <algorithm>
#include <execution>

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
            const std::map<const TypeInfo, std::unordered_set<Listener*>>::iterator it = mEventCollection.find(what);

            //If we found the Event
            if (it != mEventCollection.end())
                std::for_each(std::execution::par_unseq, it->second.begin(), it->second.end(), [&who, &it](Listener* x) {
                    //If the Listener is the one we want to unsubscribe
                    if (typeid(*x).name() == typeid(who).name())
                        (*it).second.erase(x);
                });
        }

        // ------------------------------------------------------------------------
        /*! Trigger Event
        *
        *  Once an event is called, it will call every single subscribed Listener
        */ // --------------------------------------------------------------------
        void EventDispatcher::TriggerEvent(const Event& event) const {
            const std::map<const TypeInfo, std::unordered_set<Listener*>>::const_iterator it = mEventCollection.find(event);

            //If we found the Event
            if (it != mEventCollection.end())
                std::for_each(std::execution::par_unseq, it->second.begin(), it->second.end(), [&event](Listener* x) {
                    x->HandleEvent(event);
                });
        }

        std::map<const TypeInfo, std::unordered_set<Listener*>> EventDispatcher::mEventCollection;
    }
}