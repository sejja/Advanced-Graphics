#ifndef KEYBINDING_H
#define KEYBINDING_H


#include "Dependencies/ImGui/imgui.h"

class KeyBinding {
public:
    static void CheckKeyBindings();

private:
    static bool ctrlZPressed;
};

#endif // KEYBINDING_H
