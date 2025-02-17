﻿//
//	SSAONoise.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/04/24
//	Copyright � 2024 . All Rights reserved
//

#ifndef _SSAO_NOISE__H_
#define _SSAO_NOISE__H_

#include <vector>
#include <glew.h>
#include <glm.hpp>

namespace Graphics {
	namespace Architecture {
		namespace SSAO {
			class SSAONoise {
			#pragma region //Constructor
			public:
				SSAONoise();
			#pragma endregion

			#pragma region //Methods
				std::vector<glm::vec3> ZTangentNoise();
				GLuint inline GetNoiseTexture() const noexcept;
			#pragma endregion

			#pragma region //Members
			private:
				GLuint mNoiseTexture;
			#pragma endregion
			};

			// ------------------------------------------------------------------------
			/*! Get Noise Texture
			*
			*  Returns the Noise Texture generated
			*/ //----------------------------------------------------------------------
			GLuint inline SSAONoise::GetNoiseTexture() const noexcept {
				return mNoiseTexture;
			}
		}
	}
}

#endif