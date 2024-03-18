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
        void EventDispatcher::Subscribe(Listener& who, const TypeInfo& what, const function_t& fun) {
            mEventCollection[what].insert(std::make_pair(&who, fun));
        }

        // ------------------------------------------------------------------------
        /*! Unsubscribe
        *
        *  Unsubscribes a Listener from a specific Event
        */ // --------------------------------------------------------------------
        void EventDispatcher::Unsubscribe(Listener& who, const TypeInfo& what) {
            const container_t::iterator it = mEventCollection.find(what);

            //If we found the Event
            if (it != mEventCollection.end())
                //Erase the Listener from the Event
                for(auto& x : it->second)
                    //If we found the Listener
                    if (typeid(*x.first).name() == typeid(who).name()) {
                        it->second.erase(x.first);
						break;
                    }
        }

        // ------------------------------------------------------------------------
        /*! Trigger Event
        *
        *  Once an event is called, it will call every single subscribed Listener
        */ // --------------------------------------------------------------------
        void EventDispatcher::TriggerEvent(const Event& event) const {
            const container_t::const_iterator it = mEventCollection.find(event);

            //If we found the Event
            if (it != mEventCollection.end())
                std::for_each(std::execution::par_unseq, it->second.begin(), it->second.end(), [&event](const std::pair<Listener*, function_t>& x) {
                    x.second(event);
                });
        }

        EventDispatcher::container_t EventDispatcher::mEventCollection;
    }
}