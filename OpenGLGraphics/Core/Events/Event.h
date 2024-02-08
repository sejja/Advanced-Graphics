//
//	Event.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 23/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _EVENT__H_
#define _EVENT__H_

#include <map>
#include <string>
#include <typeinfo>
#include <memory>

namespace Core {
    namespace Events {
        class Event {
        public:
#pragma region //Make Pure Virtual
            virtual ~Event();
#pragma endregion
        };

        class HandlerFunction {
        public:
#pragma region //Functions
            void mHandle(const Event& concrete_event);
            virtual void Call(const Event& concrete_event) = 0;
#pragma endregion
        };

        template <typename T, typename EVENT>
        class MemberFunctionHandler : public HandlerFunction {
        public:
#pragma region //Constructors
            typedef void (T::* MemberFunction)(const EVENT&);
            MemberFunctionHandler(T* who, MemberFunction function) : mWho(who), mWhat(function) {}

        private:
            inline void Call(const Event& event);
#pragma endregion

#pragma region //Members
            T* mWho;
            MemberFunction mWhat;
#pragma endregion
        };

        class EventHandler {
        public:
#pragma region //Functions       
            template <typename T, typename EVENT>
            inline void RegisterHandler(T& who, void (T::* function)(const EVENT&));

            void mHandle(const Event& event);
#pragma endregion
        private:
#pragma region //Variables
            std::map<std::string, std::unique_ptr<HandlerFunction>> mHandlerCollection;
#pragma endregion
        };

        // ------------------------------------------------------------------------
        /*! Call
        *
        *	Calls the "What" function, on the "Who" object, with a given event
        */ // ---------------------------------------------------------------------
        template<typename T, typename EVENT>
        inline void MemberFunctionHandler<T, EVENT>::Call(const Event& event) {
            (mWho->*mWhat)(static_cast<const EVENT&>(event));
        }

        // ------------------------------------------------------------------------
        /*! Register Handler
        *
        *	Registers a "What", and a "Who", to an specific event
        */ // ---------------------------------------------------------------------
        template<typename T, typename EVENT>
        inline void EventHandler::RegisterHandler(T& who, void(T::* function)(const EVENT&)) {
            mHandlerCollection[typeid(EVENT).mName()] = std::move(std::make_unique(MemberFunctionHandler<T, EVENT>(&who, function)));
        }
    }
}


#define EVENT_DECL(a)  struct a : public Core::Events::Event {};

#endif