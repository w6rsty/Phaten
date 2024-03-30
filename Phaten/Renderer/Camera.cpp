#include "Camera.hpp"

#include "Math/Space.hpp"

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
    m_ProjectionOrtho(1.0f)
{
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

const Matrix4& Camera::GetProjection() const
{
    return m_IsPerspective ? m_ProjectionPersp : m_ProjectionOrtho;
}

} // namespace Pt