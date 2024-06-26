﻿//
//  SDL_Window.cpp
//  OpenGL Graphics
//
//  Created by Diego Revilla on 13/03/24
//  Copyright � 2024. All rights reserved.
//

#include <glew.h>
#include "Core/Window/SDLWindow.h"
#include "Dependencies/ImGui/imgui_impl_opengl3.h"
#include "Dependencies/ImGui/imgui_impl_sdl2.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"

namespace Core {
    namespace Window {
        // ------------------------------------------------------------------------
    /*! Constructor
    *
    *   Initializes the Destructor (What a thing)
    */ //----------------------------------------------------------------------
        SDLWindow::SDLWindow() noexcept :
            mWindow(nullptr, SDL_DestroyWindow), mDimensions({ 0, 0 }), mContext(NULL) {
        }
        
        // ------------------------------------------------------------------------
        /*! Destructor
        *
        *   Destroys the Window, and the context too
        */ //----------------------------------------------------------------------
        SDLWindow::~SDLWindow() {
            SDL_GL_DeleteContext(mContext);
            SDL_Quit();
        }

        // ------------------------------------------------------------------------
        /*! Create
        *
        *   Creates the window buffer on SDL. as well as a OpenGL Context
        */ //----------------------------------------------------------------------
        void SDLWindow::Create() {
            //If there is an error creating the 
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER))
                throw SDLWindowException(SDL_GetError());

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            //SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
            //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

            mWindow.reset(SDL_CreateWindow("Real Engine",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                mDimensions.x, mDimensions.y,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));

            //If we got no pointer to the window, throw an exception
            if (!mWindow)
                throw SDLWindowException("Window could not be created");

            mContext = SDL_GL_CreateContext(mWindow.get());

            SDL_GL_MakeCurrent(mWindow.get(), mContext);
            SDL_GL_SetSwapInterval(1);

            //If there was any error, throw an exception
            if (glewInit() != GLEW_OK)
                throw SDLWindowException("Failed to initialize OpenGL loader!");

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

            //Load font

            io.Fonts->AddFontDefault();
            ImFont* mainFont = io.Fonts->AddFontFromFileTTF("Content/Fonts/DroidSans.ttf", 16.5f, NULL, io.Fonts->GetGlyphRangesDefault());
            IM_ASSERT(mainFont != NULL);


            io.FontGlobalScale = 1.0f;
            io.FontDefault = mainFont;

            ImFontConfig config;
            config.MergeMode = true;
            config.GlyphMinAdvanceX = 13.0f; // Makes icons monospaced
            static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
            io.Fonts->AddFontFromFileTTF("Content/Fonts/fa-regular-400.ttf", 16.5f, &config, icon_ranges);
            io.Fonts->AddFontFromFileTTF("Content/Fonts/fa-brands-400.ttf", 16.5f, &config, icon_ranges);
            io.Fonts->AddFontFromFileTTF("Content/Fonts/fa-solid-900.ttf", 16.5f, &config, icon_ranges);



            // Setup Platform/Renderer backends
            ImGui_ImplSDL2_InitForOpenGL(mWindow.get(), mContext);
            ImGui_ImplOpenGL3_Init("#version 450");

        }

        // ------------------------------------------------------------------------
        /*! Present
        *
        *   Presents the Window, and retrieves any event needded
        */ //----------------------------------------------------------------------
        bool SDLWindow::Present() {
            SDL_Event event;

            SDL_GL_SwapWindow(mWindow.get());

            // Poll and handle events (inputs, window resize, etc.)
            while (SDL_PollEvent(&event))
                ImGui_ImplSDL2_ProcessEvent(&event);

            return !(event.type == SDL_QUIT);
        }

        // ------------------------------------------------------------------------
        /*! Set Dimension
        *
        *   Set Dimensions of the Window
        */ //----------------------------------------------------------------------
        void SDLWindow::SetDimensions(const glm::lowp_u16vec2& dim) {
            mDimensions = dim;
            SDL_SetWindowSize(mWindow.get(), dim.x, dim.y);
        }
    }
}