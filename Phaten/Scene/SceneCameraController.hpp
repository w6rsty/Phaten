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

    SceneCameraController(const SharedPtr<Camera>& camera);

    void OnEvent(float delta);

    void SetPosition(const Vector3& pos);
    void SetRotation(int x, int y);

    void Move(SceneCameraMovement dir, float speed);    
private:
    SharedPtr<Camera> m_Camera;
    Vector3 m_Position;
    Vector3 m_Rotation;
};

} // namespace Pt