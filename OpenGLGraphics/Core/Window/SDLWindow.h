﻿//
//  SDL_Window.h
//  OpenGL Graphics
//
//  Created by Diego Revilla on 13/03/24
//  Copyright � 2024. All rights reserved.
//

#ifndef _SDL_WINDOW__H_
#define _SDL_WINDOW__H_

#include "Window.h"
#include <SDL.h>
#include "CommonDefines.h"

#undef main

namespace Core {
	namespace Window {
		class SDLWindow : public Window {
		public:
#pragma region Constructors & Destructors
			SDLWindow() noexcept;
			~SDLWindow();
			CLASS_EXCEPTION(SDLWindow)
#pragma endregion

#pragma region Methods
				void Create() override;
			bool Present() override;
#pragma endregion

#pragma region Getters & Setters
			void SetDimensions(const glm::lowp_u16vec2& dim) override;
			DONTDISCARD glm::lowp_u16vec2 inline GetDimensions() const;
#

		private:
#pragma region Members
			SDL_GLContext mContext;
			std::deleted_pointer<SDL_Window> mWindow;
			glm::lowp_u16vec2 mDimensions;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Dimensions
		*
		*   Returns the Dimensions of the Window
		*/ //----------------------------------------------------------------------
		glm::lowp_u16vec2 SDLWindow::GetDimensions() const {
			return mDimensions;
		}
	}
}

#endif