#include "TransformObject.h"
#include <iostream>



TransformObjectAction::TransformObjectAction(std::shared_ptr<Core::Object> mObject)
{
	this->mObject = mObject;
	this->mInitPos = PrevStates::GetPrevPos();
	this->mInitRot = PrevStates::GetPrevRot();
	this->mInitScale = PrevStates::GetPrevScale();
	this->mFinalPos = mObject->GetPosition();
	this->mFinalRot = mObject->GetRotation();
	this->mFinalScale = mObject->GetScale();

	//limpiar prevstates?? nuse
}
void TransformObjectAction::Undo(){
	this->mObject->SetPosition(mInitPos);
	this->mObject->SetRotation(mInitRot);
	this->mObject->SetScale(mInitScale);
}

void TransformObjectAction::Redo(){
	this->mObject->SetPosition(mFinalPos);
	this->mObject->SetRotation(mFinalRot);
	this->mObject->SetScale(mFinalScale);
}
