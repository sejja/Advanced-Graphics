//
//	Object.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/23
//	Copyright © 2023. All Rights reserved
//

#include "Object.h"
#include "Component.h"
#include "Core/PageAllocator.h"

namespace Core {
	// ------------------------------------------------------------------------
	/*! Update
	*
	*  Updates all the components within the object
	*/ // ---------------------------------------------------------------------
	void Object::Update() {
		std::for_each(std::execution::par, mComponents.begin(), mComponents.end(), [](const std::shared_ptr<Component>& y) {
			y->Update();
			});
	}
}