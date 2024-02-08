//
//	TypeInfo.cpp
//	Dragon's Lake Test Task
//
//	Created by Diego Revilla on 23/03/23
//	Copyright © 2023 Dragon's Lake. All Rights reserved
//

#include "TypeInfo.h"

namespace Core {
    namespace Events {
        // ------------------------------------------------------------------------
        /*! Custom Constructor
        *
        *   Creates a Type Info class from Any Object
        */ // ---------------------------------------------------------------------
        TypeInfo::TypeInfo(const std::type_info& obj) :
            mName(obj.name()) {}
    }
}