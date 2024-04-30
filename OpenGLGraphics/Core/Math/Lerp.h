//
//	Lerp.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef _LERP__H_
#define _LERP__H_

namespace Core {
	namespace Math {
		template<typename T>
		T Lerp(const T& a, const T& b, const T& f) {
			return a + f * (b - a);
		}
	}
}

#endif