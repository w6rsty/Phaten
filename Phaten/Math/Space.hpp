#pragma once

#include "Matrix.hpp"

namespace Pt {

// This better
void Perspective(Matrix4& mat, float fov, float aspect, float near, float far);
Matrix4 Perspective(float fov, float aspect, float near, float far);

// This better
void Orthographic(Matrix4& mat, float left, float right, float bottom, float top, float near, float far);
Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);

// This better
void LookAt(Matrix4& mat, const Vector3& from, const Vector3& to, const Vector3& up);
Matrix4 LookAt(const Vector3& from, const Vector3& to, const Vector3& up);

} // namespace Pt