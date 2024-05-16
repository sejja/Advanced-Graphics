#ifndef __MODELACTION__H__
#define __MODELACTION__H__

#include "../Action.h"
#include "Graphics/Primitives/GLBModel.h"
#include <Graphics/Primitives/Renderables.h>


class ModelAction : public Action {
public:

	ModelAction(std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> model);

	virtual void Undo() override;
	virtual void Redo() override;

private:

	std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> mModel;
	
	Core::Assets::AssetReference<::Graphics::Primitives::Model> initModel;
	Core::Assets::AssetReference<::Graphics::Primitives::Model> finalModel;

	Core::Assets::AssetReference<Core::Graphics::ShaderProgram> initShader;
	Core::Assets::AssetReference<Core::Graphics::ShaderProgram> finalShader;

};

#endif
