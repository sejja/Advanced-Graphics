//
//	RTTI.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 24/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _RTTI__H_
#define _RTTI__H_

#include "CommonDefines.h"
#include <type_traits>

namespace RTTI {
	// ------------------------------------------------------------------------
	/*! Is A
	*
	*   Is "other" a <<type>>
	*/ // ---------------------------------------------------------------------
	template<typename TYPE, typename OTHERTYPE>
	DONTDISCARD constexpr inline bool IsA(OTHERTYPE* other) noexcept {
		return dynamic_cast<TYPE*>(other);
	}

	// ------------------------------------------------------------------------
	/*! Is A
	*
	*   Is "other" a <<type>>
	*/ // ---------------------------------------------------------------------
	template<typename TYPE, typename OTHERTYPE>
	DONTDISCARD constexpr inline bool IsChild() noexcept {
		return std::is_base_of<OTHERTYPE, TYPE>::value;
	}
}

#endif