#ifndef SELECTED_OBJ_H
#define SELECTED_OBJ_H

#include <memory> 
#include "Core/ECSystem/Object.h"
#include "Graphics/Primitives/GLBModel.h"


class SelectedObj {

private:
    std::shared_ptr<Core::Object> selectedObject;
    std::shared_ptr<Core::Component> selectedComponent;
	Graphics::Primitives::Mesh* selectedMesh;

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
    /*! Get Selected Component
    *
    *  Gets the selected component
    */ // ---------------------------------------------------------------------


    std::shared_ptr<Core::Component> GetSelectedComponent() const {
		return selectedComponent;
	}

	Graphics::Primitives::Mesh* GetSelectedMesh() {
		return selectedMesh;
	}

    void SetSelectedMesh(Graphics::Primitives::Mesh* mesh) {
        selectedMesh = mesh;
    }


    // ------------------------------------------------------------------------
    /*! Set Selected Object
    *
    *  Sets the selected object to the given one
    */ // ---------------------------------------------------------------------

    void SetSelectedObject(const std::shared_ptr<Core::Object>& obj) {
        selectedObject = obj;
    }

    // ------------------------------------------------------------------------
    /*! Set Selected Component
    *
    * Sets the selected component to the given one
    */ // ---------------------------------------------------------------------

    void SetSelectedComponent(const std::shared_ptr<Core::Component>& comp) {
		selectedComponent = comp;
	}

};

#endif
