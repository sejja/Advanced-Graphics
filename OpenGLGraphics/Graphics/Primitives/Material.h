//
//	Material.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 09/08/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _MATERIAL__H_
#define _MATERIAL__H_

#include "CommonDefines.h"
#include "glm/vec4.hpp"

namespace Graphics {
	class Material {
	#pragma region //Declarations
	public:
		struct MaterialException : public std::exception {
			MaterialException(const char* message) : std::exception(message) {}
		};
	#pragma endregion
	
	#pragma region //Constructors & Destructors
		Material() = default;
		Material(const Material&) = default;
		Material(Material&&) = default;
		Material& operator=(const Material&) = default;
		Material& operator=(Material&&) = default;
		~Material() = default;
	#pragma endregion

	#pragma region //Functions
		inline void SetAmbient(const glm::vec4& ambient) noexcept;
		inline void SetDiffuse(const glm::vec4& diffuse) noexcept;
		inline void SetSpecular(const glm::vec4& specular) noexcept;
		inline void SetEmissive(const glm::vec4& emissive) noexcept;
		void SetShininess(const float n);
		DONTDISCARD constexpr inline glm::vec4 GetAmbient() const noexcept;
		DONTDISCARD constexpr inline glm::vec4 GetDiffuse() const noexcept;
		DONTDISCARD constexpr inline glm::vec4 GetSpecular() const noexcept;
		DONTDISCARD constexpr inline glm::vec4 GetEmissive() const noexcept;
		DONTDISCARD constexpr inline float GetShininess() const noexcept;
	#pragma endregion

#pragma region // Variables
	private:
		glm::vec4 mAmbient;
		glm::vec4 mEmissive;
		glm::vec4 mSpecular;
		glm::vec4 mDiffuse;
		float mShininess;
	#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Set Ambient
	*
	*  Sets the Ambient Color of a Material
	*/ // ---------------------------------------------------------------------
	void Material::SetAmbient(const glm::vec4& ambient) noexcept {
		mAmbient = ambient;
	}

	// ------------------------------------------------------------------------
	/*! Set Diffuse
	*
	*  Sets the Diffuse Color of a Material
	*/ // ---------------------------------------------------------------------
	void Material::SetDiffuse(const glm::vec4& diffuse) noexcept {
		mDiffuse = diffuse;
	}

	// ------------------------------------------------------------------------
	/*! Set Specular
	*
	*  Sets the Specular Color of a Material
	*/ // ---------------------------------------------------------------------
	void Material::SetSpecular(const glm::vec4& specular) noexcept {
		mSpecular = specular;

	}

	// ------------------------------------------------------------------------
	/*! Set Emissive
	*
	*  Sets the Emissive Color of a Material
	*/ // ---------------------------------------------------------------------
	void Material::SetEmissive(const glm::vec4& emissive) noexcept {
		mEmissive = emissive;
	}

	// ------------------------------------------------------------------------
	/*! Get Ambient
	*
	*  Returns the Ambient Color
	*/ // ---------------------------------------------------------------------
	constexpr inline glm::vec4 Material::GetAmbient() const noexcept {
		return mAmbient;
	}

	// ------------------------------------------------------------------------
	/*! Get Diffuse
	*
	*  Returns the Diffuse Color
	*/ // ---------------------------------------------------------------------
	constexpr inline glm::vec4 Material::GetDiffuse() const noexcept {
		return mDiffuse;
	}

	// ------------------------------------------------------------------------
	/*! Get Specular
	*
	*  Returns the Specular Color
	*/ // ---------------------------------------------------------------------
	constexpr inline glm::vec4 Material::GetSpecular() const noexcept {
		return mSpecular;
	}

	// ------------------------------------------------------------------------
	/*! Get Emissive
	*
	*  Returns the Emissive Color
	*/ // ---------------------------------------------------------------------
	constexpr inline glm::vec4 Material::GetEmissive() const noexcept {
		return mEmissive;
	}

	// ------------------------------------------------------------------------
	/*! Get Shininess
	*
	*  Returns the Shininess intensity
	*/ // ---------------------------------------------------------------------
	constexpr  inline float Material::GetShininess() const noexcept {
		return mShininess;
	}
}

#endif