//
//	Object.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/24
//	Copyright © 2024. All Rights reserved
//

#include "Object.h"
#include "Component.h"

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