#include "MeshAction.h"

MeshAction::MeshAction(Graphics::Primitives::Mesh* mesh)
{
	this->mMesh = mesh;

	this->initNormal = PrevStates::GetPrevNormal();
	this->initDiffuse = PrevStates::GetPrevDiffuse();

	this->finalNormal = mesh->getNormal();
	this->finalDiffuse = mesh->getDiffuse();
}

void MeshAction::Undo(){
	mMesh->setNormal(initNormal);
	mMesh->setDiffuse(initDiffuse);
}

void MeshAction::Redo(){
	mMesh->setNormal(finalNormal);
	mMesh->setDiffuse(finalDiffuse);
}
