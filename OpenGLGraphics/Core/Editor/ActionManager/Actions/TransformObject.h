#ifndef __TRANSFORMOBJECTACTION__H__
#define __TRANSFORMOBJECTACTION__H__

#include "../Action.h"

class TransformObjectAction : public Action {
public:

	TransformObjectAction(std::shared_ptr<Core::Object> mObject);
   
    virtual void Undo() override;
    virtual void Redo() override;

private:
	std::shared_ptr<Core::Object> mObject;
    

    glm::vec3 mInitPos;
	glm::vec3 mInitRot;
	glm::vec3 mInitScale;

	glm::vec3 mFinalPos;
	glm::vec3 mFinalRot;
	glm::vec3 mFinalScale;

};

#endif
