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

	std::vector <std::shared_ptr<Action>> GetActions() { return actions; }
	int GetCurrentAction() { return currentAction; }



private:
	std::vector<std::shared_ptr<Action>> actions;
	int currentAction = -1;
};




#endif