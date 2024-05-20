#include "KeyBinding.h"
#include "Editor.h"

bool KeyBinding::ctrlZPressed = false;

void KeyBinding::CheckKeyBindings() {
    if (ImGui::GetIO().KeysDown[ImGui::GetKeyIndex(ImGuiKey_Z)] && ImGui::GetIO().KeyCtrl) {
        if (!ctrlZPressed) {
            Singleton<::Editor>::Instance().GetActionManager()->Undo();
            ctrlZPressed = true;
        }
    }
    else if (ImGui::GetIO().KeysDown[ImGui::GetKeyIndex(ImGuiKey_Z)] &&
        ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift) {
        Singleton<::Editor>::Instance().GetActionManager()->Redo();
        ctrlZPressed = false; 
    }
    else {
        ctrlZPressed = false; 
    }
}
