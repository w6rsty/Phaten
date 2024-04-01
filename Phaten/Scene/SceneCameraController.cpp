#include "SceneCameraController.hpp"

#include <SDL.h>

#include "Math/Space.hpp"

namespace Pt {

SceneCameraController::SceneCameraController() :
    m_Position(Vector3::ZERO),
    m_Rotation(Vector3 {0.0, -90.0, 0.0})
{
}

void SceneCameraController::Attach(const SharedPtr<Camera>& camera)
{
    m_Camera = camera;
    m_Position = m_Camera->GetPosition();
    m_Rotation = m_Camera->GetRotation();
}

void SceneCameraController::Detach()
{
    m_Camera.Reset();
    m_Position = Vector3::ZERO;
    m_Rotation = Vector3 {0.0, -90.0, 0.0};
}

void SceneCameraController::SetPosition(const Vector3& pos)
{
    m_Position = pos;
    m_Camera->SetPosition(m_Position);
}

void SceneCameraController::SetRotation(const Vector3& rot)
{
    m_Camera->SetRotation(rot);
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

void SceneCameraController::Rotate(const Vector3& move, float speed)
{
    m_Rotation += move * speed;
    
    m_Camera->SetRotation(m_Rotation);
}

} // namespace Pt