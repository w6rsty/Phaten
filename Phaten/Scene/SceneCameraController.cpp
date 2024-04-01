#include "SceneCameraController.hpp"

#include <SDL.h>

#include "Math/Space.hpp"

namespace Pt {

static const float MOVE_SENSITIVITY = 2.0f;
static const float ROTATE_SENSITIVITY = 0.1f;

SceneCameraController::SceneCameraController(const SharedPtr<Camera>& camera) :
    m_Camera(camera),
    m_Position(Vector3::ZERO),
    m_Rotation(Vector3 {0.0, -90.0, 0.0})
{
    m_Camera->SetPerspective(45.0f, 0.1f, 100.0f);
}

void SceneCameraController::OnEvent(float delta)
{
    float speed = MOVE_SENSITIVITY * delta;
    
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    if (keyState[SDL_SCANCODE_W])
        Move(SceneCameraMovement::FORWARD, speed);
    if (keyState[SDL_SCANCODE_S])
        Move(SceneCameraMovement::BACKWARD, speed);
    if (keyState[SDL_SCANCODE_A])
        Move(SceneCameraMovement::LEFT, speed);
    if (keyState[SDL_SCANCODE_D])
        Move(SceneCameraMovement::RIGHT, speed);
}

void SceneCameraController::SetPosition(const Vector3& pos)
{
    m_Position = pos;
    m_Camera->SetPosition(m_Position);
}

void SceneCameraController::Move(SceneCameraMovement dir, float speed)
{
    switch (dir)
    {
    case SceneCameraMovement::FORWARD:
        m_Position += m_Camera->GetDirection() * speed;
        break;
    case SceneCameraMovement::BACKWARD:
        m_Position -= m_Camera->GetDirection() * speed;
        break;
    case SceneCameraMovement::LEFT:
        m_Position -= m_Camera->GetDirection().Cross(Vector3::UP) * speed;
        break;
    case SceneCameraMovement::RIGHT:
        m_Position += m_Camera->GetDirection().Cross(Vector3::UP) * speed;
        break;
    }

    m_Camera->SetPosition(m_Position);
}

void SceneCameraController::SetRotation(int x, int y)
{
    m_Rotation.x += (float)x;
    m_Rotation.y += (float)y;
    m_Camera->SetRotation(m_Rotation);
}

} // namespace Pt