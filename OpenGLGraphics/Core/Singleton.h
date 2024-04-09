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
#pragma region //Construtors&Destructor
    Singleton();
    ~Singleton();
public:
    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;
#pragma endregion
#pragma region //Functions
    constexpr DONTDISCARD static T& Instance();
#pragma endregion

};

// ------------------------------------------------------------------------
/*! Constructor
*
*   EMPTY FUNCTION
*/ //----------------------------------------------------------------------
template<typename T>
Singleton<T>::Singleton() {}

// ------------------------------------------------------------------------
/*! Destructor
*
*   EMPTY FUNCTION
*/ //----------------------------------------------------------------------
template<typename T>
Singleton<T>::~Singleton() {}

// ------------------------------------------------------------------------
/*! Instance
*
*   Returns THE instance of the global object
*/ //----------------------------------------------------------------------
template<typename T>
constexpr T& Singleton<T>::Instance() {
    static T instance;

    return instance;
}

#endif