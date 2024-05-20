#ifndef __ACTION__H__
#define __ACTION__H__

#include <memory> 
#include "Core/ECSystem/Object.h"
#include "Graphics/Primitives/GLBModel.h"
#include "PrevStates.h"

class Action {

public:
	~Action();

	virtual void Undo() = 0;
	virtual void Redo() = 0;


};




#endif