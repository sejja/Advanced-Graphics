//
//	EventDispatcher.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 05/03/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _EVENT_DISPATCHER__H_
#define _EVENT_DISPATCHER__H_

#include "Event.h"
#include "TypeInfo.h"
#include <map>
#include <unordered_set>

namespace Core {
    namespace Events {
        class Listener {
        public:
#pragma region //Functions
            virtual void HandleEvent(const Event&);
#pragma endregion
        };

        class EventDispatcher {
        public:
#pragma region //Functions
			using function_t = std::function<void(const Event&)>;
            using container_t = std::map<const TypeInfo, std::unordered_map<Listener*, function_t>>;
#pragma endregion
#pragma region //Functions
            void Subscribe(Listener& who, const TypeInfo& what, const function_t& fun);
            void Unsubscribe(Listener& who, const TypeInfo& what);
            void TriggerEvent(const Event& event) const;
#pragma endregion

        private:
#pragma region //Variables
            static container_t mEventCollection;
#pragma endregion
        };
    }
}

#endif