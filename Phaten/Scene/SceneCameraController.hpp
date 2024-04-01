#pragma once

#include "Object/Ptr.hpp"
#include "Renderer/Camera.hpp"

union SDL_Event;

namespace Pt {

enum class SceneCameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class SceneCameraController : public RefCounted
{
public:
    SceneCameraController();

    void Attach(const SharedPtr<Camera>& camera);
    void Detach();
    bool IsAttached() const { return m_Camera.IsNull(); }

    void OnEvent(float delta);

    void SetPosition(const Vector3& pos);
    void SetRotation(const Vector3& rot);

    void Move(SceneCameraMovement dir, float speed = 1.0f);    
    void Rotate(const Vector3& move, float speed = 1.0f);
private:
    SharedPtr<Camera> m_Camera;
    Vector3 m_Position;
    Vector3 m_Rotation;
};

} // namespace Pt