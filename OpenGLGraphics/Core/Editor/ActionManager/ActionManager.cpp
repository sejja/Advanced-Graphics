#include "ActionManager.h"




ActionManager::ActionManager()
{
}

ActionManager::~ActionManager()
{
}

void ActionManager::AddAction(std::shared_ptr<Action> action){
	actions.resize(currentAction + 1);// si hay acciones futuras, las borra
	actions.push_back(action);
	currentAction++;
}

void ActionManager::Undo()
{
	if (canUndo()) {
		actions[currentAction]->Undo();
		currentAction--;
	}
}

void ActionManager::Redo()
{
	if (canRedo()) {
		currentAction++;
		actions[currentAction]->Redo();
	}
}