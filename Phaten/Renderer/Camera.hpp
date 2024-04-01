#pragma once

#include "Object/Ptr.hpp"
#include "Math/Matrix.hpp"

namespace Pt {

class Camera : public RefCounted
{
public:
    Camera();

    /// Set the camera to perspective mode
    void SetPerspective(float fov, float near, float far);
    void SetOrthographic(float size, float near, float far);
    /// Check if the camera is in perspective mode
    bool IsPerspective() const { return m_IsPerspective; }

    // Setter
    void SetProjMode(bool persp) { m_IsPerspective = persp; }
    void SetAspectRatio(float aspect) { m_AspectRatio = aspect; UpdateProjection(); }
    void SetFov(float fov) { m_Fov = fov; UpdateProjection(); }
    void SetOrthoSize(float size) { m_OrthoSize = size; UpdateProjection(); }
    void SetNearPersp(float near) { m_NearPersp = near; UpdateProjection(); }
    void SetFarPersp(float far) { m_FarPersp = far; UpdateProjection(); }
    void SetNearOrtho(float near) { m_NearOrtho = near; UpdateProjection(); }
    void SetFarOrtho(float far) { m_FarOrtho = far; UpdateProjection(); }

    // Getters
    float GetAspectRatio() const { return m_AspectRatio; }
    float GetFov() const { return m_Fov; }
    float GetOrthoSize() const { return m_OrthoSize; }
    float GetNearPersp() const { return m_NearPersp; }
    float GetFarPersp() const { return m_FarPersp; }
    float GetNearOrtho() const { return m_NearOrtho; }
    float GetFarOrtho() const { return m_FarOrtho; }

    const Vector3& GetPosition() const { return m_Position; }
    const Vector3& GetDirection() const { return m_Direction; }
    const Vector3& GetRotation() const { return m_Rotation; }

    void SetPosition(const Vector3& pos);
    void SetRotation(const Vector3& rot);

    const Matrix4& GetProjection() const;
    const Matrix4& GetView() const;
private:
    void UpdateProjection();
    void UpdateView();

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

    Matrix4 m_View;

    Vector3 m_Position;
    Vector3 m_Direction;
    Vector3 m_Rotation;
    Vector3 m_Up;
};

} // namespace Pt