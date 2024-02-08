//
//	TypeInfo.h
//	Dragon's Lake Test Task
//
//	Created by Diego Revilla on 23/03/23
//	Copyright © 2023 Dragon's Lake. All Rights reserved
//

#ifndef _TYPE_INFO__H_
#define _TYPE_INFO__H_

#include <typeinfo>
#include "CommonDefines.h"

namespace Core {
    namespace Events {
        class TypeInfo {
#pragma region //Constructor
        public:
            template <typename T>
            TypeInfo(const T& obj);

            TypeInfo(const std::type_info& tpnfo);
#pragma endregion

#pragma region //Functions
            DONTDISCARD inline bool operator==(const TypeInfo& rhs) const;
            DONTDISCARD inline bool operator!=(const TypeInfo& rhs) const;
            DONTDISCARD inline bool operator<(const TypeInfo& rhs) const;
#pragma endregion

#pragma region //Variables
        private:
            const char* mName;
#pragma endregion
        };

        // ------------------------------------------------------------------------
        /*! Custom Constructor
        *
        *   Creates a Type Info class from Any Object
        */ // ---------------------------------------------------------------------
        template<typename T>
        TypeInfo::TypeInfo(const T& obj) :
            mName(typeid(obj).name()) {}

        // ------------------------------------------------------------------------
        /*! Operator ==
        *
        *   Checks if two TypeInfo's are the same
        */ // ---------------------------------------------------------------------
        bool TypeInfo::operator==(const TypeInfo& rhs) const {
            return mName == rhs.mName;
        }

        // ------------------------------------------------------------------------
        /*! Operator !=
        *
        *   Checks if two TypeInfo's are different
        */ // ---------------------------------------------------------------------
        bool TypeInfo::operator!=(const TypeInfo& rhs) const {
            return mName != rhs.mName;
        }

        // ------------------------------------------------------------------------
        /*! Operator <
        *
        *   Compares two Type Info's (So we can use it with a map)
        */ // ---------------------------------------------------------------------
        bool TypeInfo::operator<(const TypeInfo& rhs)const {
            return mName < rhs.mName;
        }
    }
}

#endif