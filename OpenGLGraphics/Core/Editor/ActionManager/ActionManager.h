#ifndef __ACTIONMANAGER__H__
#define __ACTIONMANAGER__H__

#include <vector>
#include "Action.h"

class ActionManager {

public:
	ActionManager();
	~ActionManager();

	void AddAction(std::shared_ptr<Action> action);
	void Undo();
	void Redo();

	bool canUndo() { return currentAction > 0; }
	bool canRedo() { return (actions.size() > 0) &&  currentAction < actions.size() - 1; }

	std::vector <std::shared_ptr<Action>> GetActions() { return actions; }
	int GetCurrentAction() { return currentAction; }

	void Clear() { 
		actions.clear();
		currentAction = 0;
	};

private:
	std::vector<std::shared_ptr<Action>> actions;
	int currentAction = 0;
};




#endif