#pragma once

#include "Renderer/Camera.hpp"

namespace Pt {



class SceneCamera : public Camera
{
public:
    enum class Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    SceneCamera();

    void SetPosition(const Vector3& pos) { m_Position = pos; UpdateView(); }
    void SetRotation(const Vector3& rot) { m_Rotation = rot; UpdateView(); }

    void Move(Movement movement);
    void Rotate(float xOffset, float yOffset);

    void SetMoveSpeed(float speed) { m_MoveSpeed = speed; }
    void SetRotateSpeed(float speed) { m_RotateSpeed = speed; }

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

    float m_MoveSpeed;
    float m_RotateSpeed;

    Matrix4 m_View;
};

} // namespace Pt