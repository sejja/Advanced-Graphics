#ifndef APP_WRAPPER_H
#define APP_WRAPPER_H

#include "Core/Window/SDLWindow.h"
#include "Graphics/OpenGLPipeline.h"
#include "Core/Application.h"

class AppWrapper : public Core::GraphicApplication<SDLWindow, Core::Graphics::OpenGLPipeline> {
private:
    Core::Scene mScene;

public:
    Core::Scene& getScene() {
        return mScene;
    }
};

#endif
