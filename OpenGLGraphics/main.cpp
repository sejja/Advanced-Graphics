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

// ------------------------------------------------------------------------
/*! Main
*
*   Program Entry point
*/ //----------------------------------------------------------------------
int main() {
    struct MyOpenGLApp : public 
       Core::GraphicApplication<SDLWindow, Core::Graphics::OpenGLPipeline> {
       Core::Scene mScene;
    } app;
    
	app.mScene.CreateScene("Content/Maps/Scene.level",  app.GetPipelineRef() , [&app](const std::shared_ptr<Core::Object>& obj) {
        obj->ForEachComponent([&app](const std::shared_ptr<Core::Component>& comp) {

            /*
            std::shared_ptr<Core::Particles::ParticleMangager> particleManager = std::dynamic_pointer_cast<Core::Particles::ParticleMangager>(comp);
            if (particleManager)
            {
                app.GetPipelineRef().SetParticleManager(std::dynamic_pointer_cast<Core::Particles::ParticleMangager>(comp));
                std::cout << "Se ha anadido ParticlesManager\n";
            }
            */
            std::shared_ptr<Core::Graphics::Renderable> renderable = std::dynamic_pointer_cast<Core::Graphics::Renderable>(comp);
            //If the object is a renderable
            if (renderable) app.GetPipelineRef().AddRenderable(renderable);
            });

        });
    //app.mScene.UploadObjectsToPipeline(app.GetPipelineRef());

    app.SetTickFunction([&app]() {
		app.mScene.Tick();
    });
    
    app.Run();

    return 0;
}