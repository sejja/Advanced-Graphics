//
//	Material.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _MATERIAL__H_
#define _MATERIAL__H_

#include "CommonDefines.h"
#include "Color.h"

namespace Graphics {
	namespace Primitives {
		class Material {
#pragma region //Declarations
		public:
			CLASS_EXCEPTION(Material)
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
			inline void SetAmbient(const Color& ambient) noexcept;
			inline void SetDiffuse(const Color& diffuse) noexcept;
			inline void SetSpecular(const Color& specular) noexcept;
			inline void SetEmissive(const Color& emissive) noexcept;
			void SetShininess(const float n);
			DONTDISCARD constexpr inline Color GetAmbient() const noexcept;
			DONTDISCARD constexpr inline Color GetDiffuse() const noexcept;
			DONTDISCARD constexpr inline Color GetSpecular() const noexcept;
			DONTDISCARD constexpr inline Color GetEmissive() const noexcept;
			DONTDISCARD constexpr inline float GetShininess() const noexcept;
#pragma endregion

#pragma region // Variables
		private:
			Color mAmbient;
			Color mEmissive;
			Color mSpecular;
			Color mDiffuse;
			float mShininess;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Set Ambient
		*
		*  Sets the Ambient Color of a Material
		*/ // ---------------------------------------------------------------------
		void Material::SetAmbient(const Color& ambient) noexcept {
			mAmbient = ambient;
		}

		// ------------------------------------------------------------------------
		/*! Set Diffuse
		*
		*  Sets the Diffuse Color of a Material
		*/ // ---------------------------------------------------------------------
		void Material::SetDiffuse(const Color& diffuse) noexcept {
			mDiffuse = diffuse;
		}

		// ------------------------------------------------------------------------
		/*! Set Specular
		*
		*  Sets the Specular Color of a Material
		*/ // ---------------------------------------------------------------------
		void Material::SetSpecular(const Color& specular) noexcept {
			mSpecular = specular;

		}

		// ------------------------------------------------------------------------
		/*! Set Emissive
		*
		*  Sets the Emissive Color of a Material
		*/ // ---------------------------------------------------------------------
		void Material::SetEmissive(const Color& emissive) noexcept {
			mEmissive = emissive;
		}

		// ------------------------------------------------------------------------
		/*! Get Ambient
		*
		*  Returns the Ambient Color
		*/ // ---------------------------------------------------------------------
		constexpr inline Color Material::GetAmbient() const noexcept {
			return mAmbient;
		}

		// ------------------------------------------------------------------------
		/*! Get Diffuse
		*
		*  Returns the Diffuse Color
		*/ // ---------------------------------------------------------------------
		constexpr inline Color Material::GetDiffuse() const noexcept {
			return mDiffuse;
		}

		// ------------------------------------------------------------------------
		/*! Get Specular
		*
		*  Returns the Specular Color
		*/ // ---------------------------------------------------------------------
		constexpr inline Color Material::GetSpecular() const noexcept {
			return mSpecular;
		}

		// ------------------------------------------------------------------------
		/*! Get Emissive
		*
		*  Returns the Emissive Color
		*/ // ---------------------------------------------------------------------
		constexpr inline Color Material::GetEmissive() const noexcept {
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
}

#endif