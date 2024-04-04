#pragma once

#include "Object/Ptr.hpp"
#include "Renderer/Camera.hpp"
#include "Math/Quaternion.hpp"

union SDL_Event;

namespace Pt {

enum class SceneCameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/// Controller for scene camera.
/// Do not provide any access to camera info.
/// Provide Attach and Detach so it by .
// TODO: Add a base class for camera controller.
class SceneCameraController : public RefCounted
{
public:
    SceneCameraController();

    void Attach(const SharedPtr<Camera>& camera);
    void Detach();
    bool IsAttached() const { return m_Camera.IsNull(); }

    void OnEvent(float delta);

    void SetPosition(const Vector3& pos);
    void SetRotation(const Quaternion& quat);

    void Move(SceneCameraMovement dir, float speed = 1.0f);    
    void Rotate(float xOffset, float yOffset, float speed = 1.0f);
private:
    SharedPtr<Camera> m_Camera;
    Vector3 m_Position;
};

} // namespace Pt