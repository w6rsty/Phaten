#pragma once

#include <map>
#include <functional>

#include "Math/IntVector.hpp"

union SDL_Event;

namespace Pt {

enum ButtonState
{
    UP = 0,
    PRESSED,
    DOWN,
    RELEASED,
};

class Input
{
public:
    Input();
    ~Input();

    void Update();

    ButtonState KeyState(unsigned key) const;
    ButtonState MouseButtonState(unsigned num) const;
    bool KeyPressed(unsigned key) const { return KeyState(key) == ButtonState::PRESSED; }
    bool KeyReleased(unsigned key) const { return KeyState(key) == ButtonState::RELEASED; }
    bool KeyDown(unsigned key) const { return KeyState(key) >= ButtonState::DOWN; }

    bool MouseButtonPressed(unsigned but) const { return MouseButtonState(but) == ButtonState::PRESSED; }
    bool MouseButtonReleased(unsigned but) const { return MouseButtonState(but) == ButtonState::RELEASED; }
    bool MouseButtonDown(unsigned but) const { return MouseButtonState(but) >= ButtonState::DOWN; }

    const IntV2& MouseMove() const { return m_MouseMove; }

    void SetOnExit(std::function<void()> onExit) { m_OnExit = onExit; }
    bool ShouldExit() const {return m_ShouldExit; }

    void SetPluginUpdate(std::function<void(const SDL_Event&)> pluginUpdate) { m_PluginUpdate = pluginUpdate; }
private:
    bool m_ShouldExit;
    IntV2 m_MouseMove;

    std::map<unsigned, ButtonState> m_KeyStates;
    std::map<unsigned, ButtonState> m_MouseButtonStates;

    std::function<void()> m_OnExit;
    using PluginUpdateFn = std::function<void(const SDL_Event&)>;
    PluginUpdateFn m_PluginUpdate;
};

} // namespace Pt