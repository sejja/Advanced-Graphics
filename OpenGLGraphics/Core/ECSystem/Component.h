//
//	Component.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _COMPONENT__H_
#define _COMPONENT__H_

#include <memory>
#include "Object.h"
#include "CommonDefines.h"

namespace Core {
	class Object;

	class Component {
	public:
	#pragma region //Declarations
		CLASS_EXCEPTION(Component)
	#pragma endregion

	#pragma region //Constructors & Destructors
		Component(const std::weak_ptr<Object>& parent);
		~Component() = default;
	#pragma endregion

	#pragma region //Functions
		virtual void Update() {}
		DONTDISCARD inline std::weak_ptr<Object> GetParent() const;
	#pragma endregion

	#pragma region //Members
	private:
		std::weak_ptr<Object> mParent;
	#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get Parent
	*
	*   Returns the Parent object as a weak reference
	*/ // ---------------------------------------------------------------------
	std::weak_ptr<Object> Component::GetParent() const {
		return mParent;
	}
}

#endif