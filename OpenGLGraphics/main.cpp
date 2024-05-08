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
#include "Core/Logger.h"


// ------------------------------------------------------------------------
/*! Main
*
*   Program Entry point
*/ //----------------------------------------------------------------------
int main() {
    AppWrapper& app = Singleton<AppWrapper>::Instance();
    Core::Logger& logger = Singleton<Core::Logger>::Instance();

    /*logger.logMessage("Prueba");
    logger.logMessage(LogLevel::WARNING, "Prueba Warning");
    logger.logMessage(LogLevel::ERROR, "Prueba Error");*/

    app.SetTickFunction([&app]() {
		app.getScene().Tick();
    });
    
    app.Run();

    return 0;
}