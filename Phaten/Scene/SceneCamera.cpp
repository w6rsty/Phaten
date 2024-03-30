#include "SceneCamera.hpp"

#include "Math/Space.hpp"

namespace Pt {

static const Vector3 UP { 0.0f, 1.0f, 0.0f };
static const float MOVE_SENSITIVITY = 0.1f;
static const float ROTATE_SENSITIVITY = 0.1f;

SceneCamera::SceneCamera() :
    Camera(),
    m_View(1.0f),
    m_Position(0.0f),
    m_Direction(0.0f, 0.0f, -1.0f),
    m_Rotation(0.0f, -90.0f, 0.0f),
    m_Up(UP),
    m_MoveSpeed(MOVE_SENSITIVITY),
    m_RotateSpeed(ROTATE_SENSITIVITY)
{
    UpdateView();
}


void SceneCamera::Move(Movement movement)
{
    switch (movement)
    {
    case Movement::FORWARD:
        m_Position += m_Direction * m_MoveSpeed;
        break;
    case Movement::BACKWARD:
        m_Position -= m_Direction * m_MoveSpeed;
        break;
    case Movement::LEFT:
        m_Position -= Normalize(Cross(m_Direction, m_Up)) * m_MoveSpeed;
        break;
    case Movement::RIGHT:
        m_Position += Normalize(Cross(m_Direction, m_Up)) * m_MoveSpeed;
        break;
    }

    UpdateView();
}

void SceneCamera::Rotate(float xOffset, float yOffset)
{
    m_Rotation.x += yOffset * m_RotateSpeed;
    m_Rotation.y += xOffset * m_RotateSpeed;

    if (m_Rotation.x > 89.0f)
        m_Rotation.x = 89.0f;
    if (m_Rotation.x < -89.0f)
        m_Rotation.x = -89.0f;

    UpdateView();
}

void SceneCamera::UpdateView()
{
    Vector3 front;
    front.x = cos(Radians(m_Rotation.y)) * cos(Radians(m_Rotation.x));
    front.y = sin(Radians(m_Rotation.x));
    front.z = sin(Radians(m_Rotation.y)) * cos(Radians(m_Rotation.x));
    m_Direction = Normalize(front);

    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Vector3 right = Normalize(Cross(m_Direction, UP));  
    // Update up
    m_Up = Normalize(Cross(right, m_Direction));

    LookAt(m_View, m_Position, m_Position + m_Direction, m_Up);
}

} // namespace Pt