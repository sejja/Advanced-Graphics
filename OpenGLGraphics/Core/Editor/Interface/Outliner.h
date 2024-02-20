#ifndef __OUTLINER__H__
#define __OUTLINER__H__

#include <iostream>
#include <glew.h>
#include "Core/ECSystem/Object.h"

class Outliner {
public:
    void Render();
    std::shared_ptr<Core::Object> GetSelectedObj();

};


#endif 
