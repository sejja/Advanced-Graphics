//
//	Object.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _OBJECT__H_
#define _OBJECT__H_

#include <memory>
#include <vector>
#include <algorithm>
#include <execution>
#include "Core/Math/Transform.h"
#include "CommonDefines.h"
#include "Core/RTTI.h"

namespace Core {
	class Component;

	class Object {
	#pragma region //Declarations
	public:
		CLASS_EXCEPTION(Object)
	#pragma endregion

	#pragma region //Functions
		inline void SetPosition(const glm::vec3& pos);
		inline void SetRotation(const glm::vec3& rot);
		inline void SetScale(const glm::vec3& scale);
		DONTDISCARD inline glm::vec3 GetPosition() const;
		DONTDISCARD inline glm::vec3 GetRotation() const;
		DONTDISCARD inline glm::vec3 GetScale() const;
		inline void ForEachComponent(std::function<void(const std::shared_ptr<Core::Component>& comp)> func);
		void Update();

		template<typename T>
		void AddComponent(std::weak_ptr<T>&& component);

		template<typename T>
		void AddComponent(std::shared_ptr<T>&& component);

		template<typename T>
		DONTDISCARD inline std::shared_ptr<T> GetComponent() const;
	#pragma endregion

	#pragma region //Members
	private:
		Mathematics::Transformation mTransform;
		std::vector<std::shared_ptr<Component>> mComponents;
	#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Set Position
	*
	*  Sets the position of a given object
	*/ // ---------------------------------------------------------------------
	void Object::SetPosition(const glm::vec3& pos) {
		mTransform.mPostion = pos;
	}

	// ------------------------------------------------------------------------
	/*! Set Rotation
	*
	*  Set the rotation of an object
	*/ // ---------------------------------------------------------------------
	void Object::SetRotation(const glm::vec3& rot) {
		mTransform.mRotation = rot;
	}

	// ------------------------------------------------------------------------
	/*! Set Scale
	*
	*  Sets thescale of an object
	*/ // ---------------------------------------------------------------------
	void Object::SetScale(const glm::vec3& sca) {
		mTransform.mScale = sca;
	}

	// ------------------------------------------------------------------------
	/*! Get Position
	*
	*  Gets the position of an object
	*/ // ---------------------------------------------------------------------
	glm::vec3 Object::GetPosition() const {
		return mTransform.mPostion;
	}

	// ------------------------------------------------------------------------
	/*! Get Rotation
	*
	*  Gets the Rotation of an object
	*/ // ---------------------------------------------------------------------
	glm::vec3 Object::GetRotation() const {
		return mTransform.mRotation;
	}

	// ------------------------------------------------------------------------
	/*! Get Scale
	*
	*  Get the scale of an object
	*/ // ---------------------------------------------------------------------
	glm::vec3 Object::GetScale() const {
		return mTransform.mScale;
	}

	// ------------------------------------------------------------------------
	/*! Add Component
	*
	*  Add Components to the object
	*/ // ---------------------------------------------------------------------
	template<typename T>
	void Object::AddComponent(std::weak_ptr<T>&& component) {
		//We should always add components
		if (!RTTI::IsChild<T, Component>())
			throw ObjectException("The given class is not a component");

		//If the component pointer is expired
		if (component.expired())
			throw ObjectException("You mustn't add expired components");

		mComponents.push_back(component.lock());
	}

	// ------------------------------------------------------------------------
	/*! Add Component
	*
	*  Add Components to the object
	*/ // ---------------------------------------------------------------------
	template<typename T>
	void Object::AddComponent(std::shared_ptr<T>&& component) {
		//We should always add components
		if (!RTTI::IsChild<T, Component>())
			throw ObjectException("The given class is not a component");

		mComponents.push_back(component);
	}

	// ------------------------------------------------------------------------
	/*! Get Component
	*
	*  Returns a component by the type
	*/ // ---------------------------------------------------------------------
	template<typename T>
	std::shared_ptr<T> Object::GetComponent() const {
		//It won't make sense not to access a component
		if(!RTTI::IsChild<T, Component>())
			throw ObjectException("The given class is not a component");

		//Look for all components
		for(auto& x : mComponents)
			if(RTTI::IsA<T>(x.get()))
				return x;

		throw ObjectException("No such component");
	}

	// ------------------------------------------------------------------------
	/*! For Each Component
	*
	*  Executes the function on each component
	*/ // ---------------------------------------------------------------------
	void Object::ForEachComponent(std::function<void(const std::shared_ptr<Core::Component>& comp)> func) {
		std::for_each(std::execution::unseq, mComponents.begin(), mComponents.end(), func);
	}
}

#endif