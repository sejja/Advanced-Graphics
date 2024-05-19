#ifndef __FIREACTION__H__
#define __FIREACTION__H__

#include "../Action.h"
#include "Core/ParticleSystem/FireSystem.h"

class FireAction : public Action {
public:

	FireAction(std::shared_ptr<::Core::Particles::FireSystem> fireSys);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	std::shared_ptr<::Core::Particles::FireSystem> curFireSys;

	glm::vec3 prevFireCenter;
	glm::vec3 prevFireColor;
	glm::vec3 prevFireRadius;
	float prevFireHeight;
	float prevFireGap;
	float prevParticleSize;

	glm::vec3 finalFireCenter;
	glm::vec3 finalFireColor;
	glm::vec3 finalFireRadius;
	float finalFireHeight;
	float finalFireGap;
	float finalParticleSize;
	




};

#endif
