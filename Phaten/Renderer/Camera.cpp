#include "Camera.hpp"

#include "Math/Space.hpp"
#include "Math/Vector.hpp"

namespace Pt {

static const bool DEFAULT_PROJ_MODE = true;
static const float DEFAULT_FOV = 45.0f;
static const float DEFAULT_ASPECT_RATIO = 1280.0f / 720.0f;
static const float DEFAULT_ORTHO_SIZE = 1.0f;
static const Vector2 DEFAULT_PERSP_NEAR_FAR = { 0.1f, 100.0f };
static const Vector2 DEFAULT_ORTHO_NEAR_FAR = { -1.0f, 1.0f };

Camera::Camera() : 
    m_IsPerspective(DEFAULT_PROJ_MODE),
    m_Fov(DEFAULT_FOV),
    m_AspectRatio(DEFAULT_ASPECT_RATIO),
    m_OrthoSize(DEFAULT_ORTHO_SIZE),
    m_NearPersp(DEFAULT_PERSP_NEAR_FAR.x),
    m_FarPersp(DEFAULT_PERSP_NEAR_FAR.y),
    m_NearOrtho(DEFAULT_ORTHO_NEAR_FAR.x),
    m_FarOrtho(DEFAULT_ORTHO_NEAR_FAR.y),
    m_ProjectionPersp(1.0f),
    m_ProjectionOrtho(1.0f),
    m_View(1.0f),
    m_Position(Vector3::ZERO),
    m_Rotation(Vector3{0, -90, 0}),
    m_Direction(Vector3::FORWARD),
    m_Up(Vector3::UP)
{
    UpdateProjection();
    UpdateView();
}

void Camera::SetPerspective(float fov, float near, float far)
{
    m_IsPerspective = true;
    m_Fov = fov;
    m_NearPersp = near;
    m_FarPersp = far;

    UpdateProjection();
}

void Camera::SetOrthographic(float size, float near, float far)
{
    m_IsPerspective = false;
    m_OrthoSize = size;
    m_NearOrtho = near;
    m_FarOrtho = far;

    UpdateProjection();
}

void Camera::SetPosition(const Vector3& pos)
{
    m_Position = pos;
    UpdateView();
}

void Camera::SetRotation(const Vector3& rot)
{
    m_Rotation = rot;
    UpdateView();
}

void Camera::UpdateProjection()
{
    if (m_IsPerspective)
    {
        Perspective(m_ProjectionPersp, m_Fov, m_AspectRatio, m_NearPersp, m_FarPersp);
    }
    else
    {
        Orthographic(m_ProjectionPersp, m_OrthoSize, m_AspectRatio, m_NearOrtho, m_FarOrtho);
    }
}

void Camera::UpdateView()
{
    Vector3 front;
    front.x = cos(Radians(m_Rotation.y)) * cos(Radians(m_Rotation.x));
    front.y = sin(Radians(m_Rotation.x));
    front.z = sin(Radians(m_Rotation.y)) * cos(Radians(m_Rotation.x));
    m_Direction = Normalize(front);

    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Vector3 right = Normalize(Cross(m_Direction, Vector3::UP));  
    // Update up
    m_Up = Normalize(Cross(right, m_Direction));

    LookAt(m_View, m_Position, m_Position + m_Direction, m_Up);
}

const Matrix4& Camera::GetProjection() const
{
    return m_IsPerspective ? m_ProjectionPersp : m_ProjectionOrtho;
}

const Matrix4& Camera::GetView() const
{
    return m_View;
}

} // namespace Pt