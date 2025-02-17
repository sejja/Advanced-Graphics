//
//	Object.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/24
//	Copyright � 2024. All Rights reserved
//

#ifndef _OBJECT__H_
#define _OBJECT__H_

#include <vector>
#include <algorithm>
#include <execution>
#include "Core/Math/Transform.h"
#include "CommonDefines.h"
#include "Core/RTTI/RTTI.h"



namespace Core {
	class Component;

	class Object {
	#pragma region //Declarations
	public:
		CLASS_EXCEPTION(Object)
	#pragma endregion

	#pragma region 
		//Functions
		inline void SetID(const std::string& id);
		inline void SetPosition(const glm::vec3& pos);
		inline void SetRotation(const glm::vec3& rot);
		inline void SetScale(const glm::vec3& scale);
		inline void SetName(const std::string name);
		DONTDISCARD inline glm::vec3 GetPosition() const;
		DONTDISCARD inline glm::vec3 GetRotation() const;
		DONTDISCARD inline glm::vec3 GetScale() const;
		DONTDISCARD inline std::string GetName() const;
		DONTDISCARD inline std::string GetID() const;




		inline void ForEachComponent(std::function<void(const std::shared_ptr<Core::Component>& comp)> func);
		void Update();

		template<typename T>
		void AddComponent(std::weak_ptr<T>&& component);

		template<typename T>
		void AddComponent(std::shared_ptr<T>&& component);

		template<typename T>
		void AddComponentR(const std::shared_ptr<T>& component);

		template<typename T>
		void RemoveComponent(std::shared_ptr<T>&& component);

		template<typename T>
		void RemoveComponentR(std::shared_ptr<T>& component);


		template<typename T>
		DONTDISCARD inline std::shared_ptr<T> GetComponent() const;

		DONTDISCARD inline std::vector<std::shared_ptr<Core::Component>> GetAllComponents() const;


	#pragma endregion

	#pragma region //Members
	protected:
		Mathematics::Transformation mTransform;
		std::vector<std::shared_ptr<Component>> mComponents;
		std::string name;
		std::string id;

	#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Set ID
	*
	*  Sets the ID of a given object
	*/ // ---------------------------------------------------------------------

	void Object::SetID(const std::string& _id) {
		id = _id;
	}

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
	/*! Set Name
	*
	*  Sets the name of an object
	*/ // ---------------------------------------------------------------------
	void Object::SetName(const std::string meshName) {
		name = meshName;
	}

	// ------------------------------------------------------------------------
	/*! Get ID
	*
	*  Gets the id of an object
	*/ // ---------------------------------------------------------------------
	std::string Object::GetID() const {
		return id;
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
	/*! Get Name
	*
	*  Get the name of an object
	*/ // ---------------------------------------------------------------------
	std::string Object::GetName() const {
		return name;
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

	template<typename T>
	void Object::AddComponentR(const std::shared_ptr<T>& component) {
		//We should always add components
		if (!RTTI::IsChild<T, Component>())
			throw ObjectException("The given class is not a component");

		mComponents.push_back(component);
	}
	// ------------------------------------------------------------------------
	/*! Remove Component
	* 
	*/// ------------------------------------------------------------------------
	template<typename T>
	void Object::RemoveComponent(std::shared_ptr<T>&& component) {
		if (!RTTI::IsChild<T, Component>())
			throw ObjectException("The given class is not a component");

		auto it = std::find(mComponents.begin(), mComponents.end(), component);

		if (it != mComponents.end()) {
			mComponents.erase(it);
		}
		else {
			throw ObjectException("No such component");
		}
	}
	template<typename T>
	void Object::RemoveComponentR(std::shared_ptr<T>& component) {
		if (!RTTI::IsChild<T, Component>())
			throw ObjectException("The given class is not a component");

		auto it = std::find(mComponents.begin(), mComponents.end(), component);

		if (it != mComponents.end()) {
			mComponents.erase(it);
		}
		else {
			throw ObjectException("No such component");
		}
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
				return std::reinterpret_pointer_cast<T>(x);

		throw ObjectException("No such component");
	}

	// ------------------------------------------------------------------------
	/*! Get All Components
	*
	*  Returns the vector of all components
	*/ // ---------------------------------------------------------------------

	inline std::vector<std::shared_ptr<Core::Component>> Object::GetAllComponents() const {
		return mComponents;
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