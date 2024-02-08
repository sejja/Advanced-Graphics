//
//	InputManager.hpp
//	OpenGL Graphics
//
//	Created by Diego Revilla 14/01/23
//	Copyright � 2021 . All Rights reserved
//

#ifndef _INPUT_MANAGER__H_
#define _INPUT_MANAGER__H_

#include <Windows.h>
#include "../CommonDefines.h"
#include <glm/vec2.hpp>
#include "Core/Events/EventDispatcher.h"

namespace Core {
    class InputManager {
    public:
        using KeyCode = decltype(VK_ACCEPT);

        EVENT_DECL(V_Down);
        EVENT_DECL(Z_Down);
        EVENT_DECL(W_Down);
        EVENT_DECL(A_Down);
        EVENT_DECL(S_Down);
        EVENT_DECL(D_Down);
        EVENT_DECL(Space_Down);
        EVENT_DECL(Shift_Down);

        DONTDISCARD bool IsKeyPressed(const KeyCode key) noexcept;
        DONTDISCARD bool IsKeyReleased(const KeyCode key) noexcept;
        DONTDISCARD bool inline IsKeyDown(const KeyCode key) const noexcept;
        DONTDISCARD bool inline IsKeyUp(const KeyCode key) const noexcept;
        DONTDISCARD glm::vec2 GetMouseMovement() const noexcept;
        DONTDISCARD glm::vec2 inline GetMousePosition() const noexcept;
        void inline SetMousePosition(const glm::vec2& pos) const noexcept;
        void SetMousePosition(const unsigned w, const unsigned h) const noexcept;
        void ProcessInput() noexcept;
        DONTDISCARD static InputManager& Instance() noexcept;
    private:
        glm::vec2 GetWindowCoordinatesMousePosition() const noexcept;
        bool mKeyboardState[101];
        glm::vec2 mMousePositon;
    };

    // ------------------------------------------------------------------------
    /*! Is Key Down
    *
    *   Name says everything
    */ // ---------------------------------------------------------------------
    bool inline InputManager::IsKeyDown(const KeyCode key) const noexcept {
        return (GetAsyncKeyState(key) & (1 << 16));
    }

    // ------------------------------------------------------------------------
    /*! Is Key Up
    *
    *   Name says everything
    */ // ---------------------------------------------------------------------
    bool inline InputManager::IsKeyUp(const KeyCode key) const noexcept {
        return !IsKeyDown(key);
    }

    // ------------------------------------------------------------------------
    /*! Set Mouse Position
    *   Name says everything
    */ // ---------------------------------------------------------------------
    void inline InputManager::SetMousePosition(const glm::vec2& pos) const noexcept {
        SetMousePosition(pos.x, pos.y);
    }

    // ------------------------------------------------------------------------
    /*! Get Mouse Position
    *   Name says everything
    */ // ---------------------------------------------------------------------
    glm::vec2 inline InputManager::GetMousePosition() const noexcept {
        return mMousePositon;
    }
}

#endif