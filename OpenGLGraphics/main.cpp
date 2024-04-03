//
//	main.cpp
//	CS300
//
//	Created by Diego Revilla on 10/05/22
//	Copyright © 2022. All Rights reserved
//

#include "Core/Application.h"
#include "Core/Window/SDLWindow.h"
#include "Graphics/OpenGLPipeline.h"
#include "Core/Window/SDLWindow.h"
#include "Core/AppWrapper.h"
#include "Core/Singleton.h"

// ------------------------------------------------------------------------
/*! Main
*
*   Program Entry point
*/ //----------------------------------------------------------------------
int main() {

    AppWrapper& app = Singleton<AppWrapper>::Instance();
    
	app.getScene().CreateScene("Content/Maps/Scene.level", app.GetPipelineRef(), [&app](const std::shared_ptr<Core::Object>& obj) {
        obj->ForEachComponent([&app](const std::shared_ptr<Core::Component>& comp) {
            std::shared_ptr<Core::Graphics::Renderable> renderable = std::dynamic_pointer_cast<Core::Graphics::Renderable>(comp);
            //If the object is a renderable
            if (renderable) app.GetPipelineRef().AddRenderable(renderable);
            });
        });
    //app.mScene.UploadObjectsToPipeline(app.GetPipelineRef());

    app.getScene().Save();

    app.SetTickFunction([&app]() {
		app.getScene().Tick();
    });
    
    app.Run();

    return 0;
}