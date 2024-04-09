//
//	Application.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 18/03/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _APPLICATION__H_
#define _APPLICATION__H_

#include <functional>
#include "Core/Window.h"
#include "Core/Graphics/Pipeline.h"
#include "Core/ECSystem/Scene.h"
#include "Core/Input/InputManager.h"
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
		void SetDimensions(const glm::lowp_u16vec2& dim) noexcept;
		DONTDISCARD constexpr inline PIPELINE& GetPipeline() noexcept;
		//PIPELINE& GraphicApplication<WINDOW, PIPELINE>::GetPipeline();

	private:
		WINDOW mWindow;
		PIPELINE mPipe;
		std::function<void()> mTick;
	};

	template<class WINDOW, class PIPELINE>
	GraphicApplication<WINDOW, PIPELINE>::GraphicApplication() {
		static_assert(std::is_base_of<Core::Window, WINDOW>::value);
		static_assert(std::is_base_of<Core::Graphics::Pipeline, PIPELINE>::value);
		Singleton<Core::Assets::ResourceManager>::Instance().Initialize();
		SetDimensions({ 1600, 900 });
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
	}

	// ------------------------------------------------------------------------
	/*! Run
	*
	*   Runs a sequence of frames, ticks, and renders
	*/ //----------------------------------------------------------------------
	template<class WINDOW, class PIPELINE>
	void GraphicApplication<WINDOW, PIPELINE>::Run() {
		Core::Input::InputManager& inptmgr = Singleton<Core::Input::InputManager>::Instance();
		//Main Loop
		while (mWindow.Present()) {
			inptmgr.ProcessInput();
			//If we have a ticking function
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
	void GraphicApplication<WINDOW, PIPELINE>::SetDimensions(const glm::lowp_u16vec2& dim) noexcept {
		mWindow.SetDimensions(dim);
		mPipe.SetDimensions(dim);
	}

	// ------------------------------------------------------------------------
	/*! Get Pipeline Reference
	*
	*   Returns a Reference to the Pipeline
	*/ //----------------------------------------------------------------------
	template<class WINDOW, class PIPELINE>
	constexpr PIPELINE& GraphicApplication<WINDOW, PIPELINE>::GetPipeline() noexcept {
		return mPipe;
	}
}

#endif