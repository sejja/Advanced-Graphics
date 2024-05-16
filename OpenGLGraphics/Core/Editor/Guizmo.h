#ifndef __GUIZMO__H__
#define __GUIZMO__H__


#include "Dependencies/ImGuizmo/ImGuizmo.h"
#include "Dependencies/ImGui/imgui.h"
#include "Graphics/Camera.h"



class Guizmo {
public:
	void Render(Core::Primitives::Camera* cam);

private:
	void Draw();

	Core::Primitives::Camera* cam;

	bool isUsing = false;

};






#endif // __GUIZMO__H__