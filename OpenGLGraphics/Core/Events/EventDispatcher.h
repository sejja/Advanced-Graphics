//
//	EventDispatcher.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _EVENT_DISPATCHER__H_
#define _EVENT_DISPATCHER__H_

#include "Event.h"
#include "TypeInfo.h"
#include <unordered_map>
#include <unordered_set>

namespace Core {
    namespace Events {
        class Listener {
#pragma region //Functions
        public:
            virtual void HandleEvent(const Event&);
#pragma endregion
        };

        class EventDispatcher {
        public:
#pragma region //Functions
            void Subscribe(Listener& who, const TypeInfo& what);
            void Unsubscribe(Listener& who, const TypeInfo& what);
            void TriggerEvent(const Event& event) const;
#pragma endregion

        private:
#pragma region //Variables
            static std::map<TypeInfo, std::unordered_set<Listener*>> mEventCollection;
#pragma endregion
        };
    }
}

#endif