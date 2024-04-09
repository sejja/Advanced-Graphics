//
//	Global.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 11/04/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _GLOBAL__H_
#define _GLOBAL__H_

#include "CommonDefines.h"

template <typename T>
class Singleton {
public:
    DONTDISCARD static T& Instance();
    
protected:
    Singleton() {}
    ~Singleton() {}
   
public:
    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;
};

// ------------------------------------------------------------------------
/*! Instance
*
*   Returns THE instance of the global object
*/ //----------------------------------------------------------------------
template<typename T>
T& Singleton<T>::Instance() {
    static T instance;

    return instance;
}

#endif