#pragma once

#include "Renderer/Camera.hpp"

namespace Pt {

class SceneCamera : public Camera
{
public:
    SceneCamera();

    void SetPosition(const Vector3& pos) { m_Position = pos; UpdateView(); }
    void SetRotation(const Vector3& rot) { m_Rotation = rot; UpdateView(); }

    void Move(const Vector3& offset);
    void Rotate(float xOffset, float yOffset);

    const Vector3& GetPosition() const { return m_Position; }
    const Vector3& GetRotation() const { return m_Rotation; }
    const Vector3& GetDirection() const { return m_Direction; }

    const Matrix4& GetView() const { return m_View; }
private:
    void UpdateView();

    Vector3 m_Position;
    Vector3 m_Direction;
    Vector3 m_Rotation;
    Vector3 m_Up;

    Matrix4 m_View;
};

} // namespace Pt