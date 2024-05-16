#include "ActionManager.h"




ActionManager::ActionManager()
{
}

ActionManager::~ActionManager()
{
}

void ActionManager::AddAction(std::shared_ptr<Action> action){
	actions.resize(currentAction + 1);// si hay acciones en el futuro, las borra
	actions.push_back(action);
	currentAction++;
}

void ActionManager::Undo()
{
	if (currentAction - 1 == -1) {
		return;
	}
	std::cout << "currentActionUNDO: " << currentAction << std::endl;
	if (currentAction >= 0) {
		actions[currentAction]->Undo();
		currentAction--;
	}
}

void ActionManager::Redo()
{
	std::cout << "currentActionREDO: " << currentAction << std::endl;

	if (currentAction < actions.size() - 1) {
		currentAction++;
		actions[currentAction]->Redo();
	}
}