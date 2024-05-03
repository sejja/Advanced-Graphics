#include "ModelAction.h"


ModelAction::ModelAction(std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> model){
	this->mModel = model;

	this->initModel = PrevStates::GetPrevModel();
	this->initShader = PrevStates::GetPrevShader();

	this->finalModel = model->GetModel();
	this->finalShader = model->GetShaderProgram();

}

void ModelAction::Undo() {
	this->mModel->SetMesh(initModel);
	this->mModel->SetShaderProgram(initShader);
}

void ModelAction::Redo() {
	this->mModel->SetMesh(finalModel);
	this->mModel->SetShaderProgram(finalShader);
}

