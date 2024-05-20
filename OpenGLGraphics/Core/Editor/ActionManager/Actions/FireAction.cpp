#include "FireAction.h"

FireAction::FireAction(std::shared_ptr<::Core::Particles::FireSystem> fireSys)
{
	this->curFireSys = fireSys;
	prevFireCenter = PrevStates::GetPrevFireCenter();
	prevFireColor = PrevStates::GetPrevFireColor();
	prevFireHeight = PrevStates::GetPrevFireHeight();
	prevFireRadius = PrevStates::GetPrevFireRadius();
	prevFireGap = PrevStates::GetPrevFireGap();
	prevParticleSize = PrevStates::GetPrevParticleSize();

	finalFireCenter = curFireSys->GetSystemCenter();
	finalFireColor = curFireSys->GetBaseColor();
	finalFireHeight = curFireSys->getHeigth();
	finalFireRadius = curFireSys->GetRadiusVector();
	finalFireGap = curFireSys->GetFireGap();
	finalParticleSize = curFireSys->GetParticleSize();
}

void FireAction::Undo() {

	curFireSys->ChangeFireSize(prevFireRadius[0], prevFireRadius[1], prevFireRadius[2], prevFireGap, prevFireHeight);
	curFireSys->SetSystemCenter(prevFireCenter, curFireSys->GetParent().lock()->GetPosition());
	curFireSys->SetParticleSize(prevParticleSize);


}

void FireAction::Redo() {
	curFireSys->ChangeFireSize(finalFireRadius[0], finalFireRadius[1], finalFireRadius[2], finalFireGap, finalFireHeight);
	curFireSys->SetSystemCenter(finalFireCenter, curFireSys->GetParent().lock()->GetPosition());
	curFireSys->SetParticleSize(finalParticleSize);
}
