#include "SceneCameraController.hpp"

#include <SDL.h>

#include "Math/Space.hpp"

namespace Pt {

SceneCameraController::SceneCameraController() :
    m_Position(Vector3::ZERO)
{
}

void SceneCameraController::Attach(const SharedPtr<Camera>& camera)
{
    m_Camera = camera;
    m_Position = m_Camera->GetPosition();
}

void SceneCameraController::Detach()
{
    m_Camera.Reset();
    m_Position = Vector3::ZERO;
}

void SceneCameraController::SetPosition(const Vector3& pos)
{
    m_Position = pos;
    m_Camera->SetPosition(m_Position);
}

void SceneCameraController::SetRotation(const Quaternion& quat)
{
    m_Camera->SetRotation(quat);
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

    // Do not use UpdatePosition() here. This is a player camera.
    // Not move along the axis.
    m_Camera->SetPosition(m_Position);
}

void SceneCameraController::Rotate(float xOffset, float yOffset, float speed)
{    
    speed *= 0.003f;
    m_Camera->UpdateRotation({yOffset * speed, xOffset * speed, 0.0});
}

} // namespace Pt