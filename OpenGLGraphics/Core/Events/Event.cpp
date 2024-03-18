//
//	Event.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 05/03/24
//	Copyright © 2024. All Rights reserved
//

#include "Event.h"

namespace Core {
	namespace Events {
		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   EMPTY FUNCTION
		*/ // ---------------------------------------------------------------------
		Event::~Event() {}

		// ------------------------------------------------------------------------
		/*! Handle
		*
		*   Handles an event, which subsecuntly will call the required object
		*/ // ---------------------------------------------------------------------
		void EventHandler::mHandle(const Event& event) {
			const std::map<TypeInfo, std::unique_ptr<HandlerFunction>>::const_iterator it = mHandlerCollection.find(typeid(event));

			//If we have functions linked with that event, handle the calls
			if (it != mHandlerCollection.end()) it->second->mHandle(event);
		}

		// ------------------------------------------------------------------------
		/*! Handle
		*
		*	Calls the event' subscribed function
		*/ // ---------------------------------------------------------------------
		void HandlerFunction::mHandle(const Event& concrete_event) {
			Call(concrete_event);
		}
	}
}