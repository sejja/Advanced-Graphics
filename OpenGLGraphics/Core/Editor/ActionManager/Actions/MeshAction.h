#ifndef __MESHAACTION__H__
#define __MESHAACTION__H__

#include "../Action.h"

class MeshAction : public Action {
public:

	MeshAction(Graphics::Primitives::Mesh* mesh);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	Graphics::Primitives::Mesh* mMesh;

	Core::Assets::Asset<Core::Graphics::Texture> initNormal;
	Core::Assets::Asset<Core::Graphics::Texture> initDiffuse;

	Core::Assets::Asset<Core::Graphics::Texture> finalNormal;
	Core::Assets::Asset<Core::Graphics::Texture> finalDiffuse;




};

#endif
