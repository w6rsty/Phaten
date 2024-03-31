#pragma once

#include "Object/Ptr.hpp"
#include "Math/Matrix.hpp"

namespace Pt {

class Camera : RefCounted
{
public:
    Camera();

    /// Set the camera to perspective mode
    void SetPerspective(float fov, float near, float far);
    void SetOrthographic(float size, float near, float far);
    /// Check if the camera is in perspective mode
    bool IsPerspective() const { return m_IsPerspective; }

    // Settings
    void SetProjMode(bool persp) { m_IsPerspective = persp; }
    void SetAspectRatio(float aspect) { m_AspectRatio = aspect; UpdateProjection(); }
    void SetFov(float fov) { m_Fov = fov; UpdateProjection(); }
    void SetOrthoSize(float size) { m_OrthoSize = size; UpdateProjection(); }
    void SetNearPersp(float near) { m_NearPersp = near; UpdateProjection(); }
    void SetFarPersp(float far) { m_FarPersp = far; UpdateProjection(); }
    void SetNearOrtho(float near) { m_NearOrtho = near; UpdateProjection(); }
    void SetFarOrtho(float far) { m_FarOrtho = far; UpdateProjection(); }

    const Matrix4& GetProjection() const;
private:
    void UpdateProjection();

    bool m_IsPerspective;

    float m_AspectRatio;

    float m_Fov;
    float m_OrthoSize;

    float m_NearPersp;
    float m_FarPersp;

    float m_NearOrtho;
    float m_FarOrtho;

    Matrix4 m_ProjectionPersp;
    Matrix4 m_ProjectionOrtho;
};

} // namespace Pt