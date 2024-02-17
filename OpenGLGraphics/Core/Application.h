//
//	Application.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 16/01/23
//	Copyright © 2023. All Rights reserved
//

#ifndef _APPLICATION__H_
#define _APPLICATION__H_

#include <functional>
#include <type_traits>
#include "Core/Window.h"
#include "Core/Pipeline.h"
#include "Core/ECSystem/Scene.h"
#include "Core/ResourceManager.h"
#include "Core/InputManager.h"
#include "Core/Singleton.h"

namespace Core {
	class Application {
	public:
		virtual void Run() = 0;
	};

	template<class WINDOW, class PIPELINE>
	class GraphicApplication : protected Application {
	public:
		GraphicApplication();
		~GraphicApplication();
		void Run() override;
		inline void SetTickFunction(const std::function<void()>& tick);
		void SetDimensions(const glm::lowp_u16vec2& dim);
		inline PIPELINE& GetPipelineRef();

	private:
		WINDOW mWindow;
		PIPELINE mPipe;
		std::function<void()> mTick;
	};

	template<class WINDOW, class PIPELINE>
	GraphicApplication<WINDOW, PIPELINE>::GraphicApplication() {
		static_assert(std::is_base_of<Core::Window, WINDOW>::value);
		static_assert(std::is_base_of<Core::Pipeline, PIPELINE>::value);
		Singleton<ResourceManager>::Instance().Initialize();
		SetDimensions({ 1072, 780 });
		mWindow.Create();
		mPipe.Init();
	}

	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys the Graphics Pipeline
	*/ //----------------------------------------------------------------------
	template<class WINDOW, class PIPELINE>
	GraphicApplication<WINDOW, PIPELINE>::~GraphicApplication() {
		mPipe.Shutdown();
		Singleton<ResourceManager>::Instance().ShutDown();
	}

	// ------------------------------------------------------------------------
	/*! Run
	*
	*   Runs a sequence of frames, ticks, and renders
	*/ //----------------------------------------------------------------------
	template<class WINDOW, class PIPELINE>
	void GraphicApplication<WINDOW, PIPELINE>::Run() {
		while (mWindow.Present()) {
			Singleton<Core::InputManager>::Instance().ProcessInput();
			if (mTick) mTick();
			mPipe.PreRender();
			mPipe.Render();
			mPipe.PostRender();
		}
	}

	// ------------------------------------------------------------------------
	/*! Set Tick Function
	*
	*   Sets the function to be run every single frame
	*/ //----------------------------------------------------------------------
	template<class WINDOW, class PIPELINE>
	void GraphicApplication<WINDOW, PIPELINE>::SetTickFunction(const std::function<void()>& tick) {
		mTick = tick;
	}

	// ------------------------------------------------------------------------
	/*! Set Dimensions
	*
	*   Set the window and pipeline resolution
	*/ //----------------------------------------------------------------------
	template<class WINDOW, class PIPELINE>
	void GraphicApplication<WINDOW, PIPELINE>::SetDimensions(const glm::lowp_u16vec2& dim) {
		mWindow.SetDimensions(dim);
		mPipe.SetDimensions(dim);
	}

	// ------------------------------------------------------------------------
	/*! Get Pipeline Reference
	*
	*   Returns a Reference to the Pipeline
	*/ //----------------------------------------------------------------------
	template<class WINDOW, class PIPELINE>
	PIPELINE& GraphicApplication<WINDOW, PIPELINE>::GetPipelineRef() {
		return mPipe;
	}
}

#endif