#include "Input.hpp"

#include <SDL.h>

namespace Pt {

Input::Input() :
    m_ShouldExit(false),
    m_MouseMove(IntV2::ZERO)
{

}

Input::~Input()
{
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void Input::Update()
{
    for (auto& state : m_KeyStates)
    {
        if (state.second == ButtonState::RELEASED)
        {
            state.second = ButtonState::UP;
        }
        else if (state.second == ButtonState::PRESSED)
        {
            state.second = ButtonState::DOWN;
        }
    }

    for (auto& state : m_MouseButtonStates)
    {
        if (state.second == ButtonState::RELEASED)
        {
            state.second = ButtonState::UP;
        }
        else if (state.second == ButtonState::PRESSED)
        {
            state.second = ButtonState::DOWN;
        }
    }

    m_MouseMove = IntV2::ZERO;

    SDL_PumpEvents();
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            m_ShouldExit = true;
            if (m_OnExit)
            {
                m_OnExit();
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.repeat == 0)
            {
                m_KeyStates[event.key.keysym.sym] = ButtonState::PRESSED;
            }
            break;
        case SDL_KEYUP:
                m_KeyStates[event.key.keysym.sym] = ButtonState::RELEASED;
            break;
        case SDL_MOUSEBUTTONDOWN:
            m_MouseButtonStates[event.button.button] = ButtonState::PRESSED;
            break;
        case SDL_MOUSEBUTTONUP:
            m_MouseButtonStates[event.button.button] = ButtonState::RELEASED;
            break;
        case SDL_MOUSEMOTION:
            m_MouseMove.x = event.motion.xrel;
            m_MouseMove.y = event.motion.yrel;
            break;
        }
    }
}

ButtonState Input::KeyState(unsigned int key) const
{
    auto it = m_KeyStates.find(key);
    return it != m_KeyStates.end() ? it->second : ButtonState::UP;
}

ButtonState Input::MouseButtonState(unsigned int num) const
{
    auto it = m_MouseButtonStates.find(num);
    return it != m_MouseButtonStates.end() ? it->second : ButtonState::UP;
}

} // namespace 