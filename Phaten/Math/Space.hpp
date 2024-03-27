#pragma once

#include "Matrix.hpp"

namespace Pt {

Matrix4 Perspective(float fov, float aspect, float near, float far);
Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);

Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up);

} // namespace Pt