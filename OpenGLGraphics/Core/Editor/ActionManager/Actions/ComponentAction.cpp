#include "ComponentAction.h"

ComponentAction::ComponentAction(std::shared_ptr<Core::Component> comp , CompActionType at){

	this->mComponent = comp;

	this-> actionType = at;

}

void ComponentAction::Undo(){
	if (actionType == CompActionType::ADD) {
		mComponent->GetParent().lock()->RemoveComponentR(mComponent);
	}
	else if (actionType == CompActionType::DELETE) {
		mComponent->GetParent().lock()->AddComponentR(mComponent);
	}
	
}

void ComponentAction::Redo(){
	if (actionType == CompActionType::ADD) {
		mComponent->GetParent().lock()->AddComponentR(mComponent);
	}
	else if (actionType == CompActionType::DELETE) {
		mComponent->GetParent().lock()->RemoveComponentR(mComponent);
	}	
}


