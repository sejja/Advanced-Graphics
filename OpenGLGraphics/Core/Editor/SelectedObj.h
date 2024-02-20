#ifndef SELECTED_OBJ_H
#define SELECTED_OBJ_H

#include <memory> 
#include "Core/ECSystem/Object.h"


class SelectedObj {

private:
    std::shared_ptr<Core::Object> selectedObject;

public:

    // ------------------------------------------------------------------------
    /*! Get Selected Object
    *
    *  Gets the selected object
    */ // ---------------------------------------------------------------------

    std::shared_ptr<Core::Object> GetSelectedObject() const {
        return selectedObject;
    }


    // ------------------------------------------------------------------------
    /*! Set Selected Object
    *
    *  Sets the selected object to the given one
    */ // ---------------------------------------------------------------------

    void SetSelectedObject(const std::shared_ptr<Core::Object>& obj) {
        selectedObject = obj;
    }

};

#endif
