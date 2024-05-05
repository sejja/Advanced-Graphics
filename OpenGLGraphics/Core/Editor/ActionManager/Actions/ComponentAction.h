#ifndef __COMPACTION__H__
#define __COMPACTION__H__

#include "../Action.h"

enum class CompActionType {
	DELETE,
	ADD
};

class ComponentAction : public Action {
public:

	ComponentAction(std::shared_ptr<Core::Component> comp , CompActionType at);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	std::shared_ptr<Core::Component> mComponent;

	CompActionType actionType;




};

#endif
