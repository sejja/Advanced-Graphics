//
//	Component.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/24
//	Copyright © 2024. All Rights reserved
//

#include "Component.h"

namespace Core {
	// ------------------------------------------------------------------------
	/*! Constructor
	*
	*   Constructs a Compoment owned by a parent
	*/ // ---------------------------------------------------------------------
	Component::Component(const std::weak_ptr<Object>& parent) :
		mParent(parent) {}
}