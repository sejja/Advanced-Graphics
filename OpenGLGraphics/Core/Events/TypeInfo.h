//
//	TypeInfo.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 05/03/24
//	Copyright � 2024. All Rights reserved
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
            DONTDISCARD constexpr inline bool operator==(const TypeInfo& rhs) const;
            DONTDISCARD constexpr inline bool operator!=(const TypeInfo& rhs) const;
            DONTDISCARD constexpr inline bool operator<(const TypeInfo& rhs) const;
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
        constexpr bool TypeInfo::operator==(const TypeInfo& rhs) const {
            return mName == rhs.mName;
        }

        // ------------------------------------------------------------------------
        /*! Operator !=
        *
        *   Checks if two TypeInfo's are different
        */ // ---------------------------------------------------------------------
        constexpr bool TypeInfo::operator!=(const TypeInfo& rhs) const {
            return mName != rhs.mName;
        }

        // ------------------------------------------------------------------------
        /*! Operator <
        *
        *   Compares two Type Info's (So we can use it with a map)
        */ // ---------------------------------------------------------------------
        constexpr bool TypeInfo::operator<(const TypeInfo& rhs) const {
            return mName < rhs.mName;
        }
    }
}

#endif