#pragma once

#include "Matrix.hpp"

namespace Pt {

Matrix3 ScaleMatrix3(const Vector3& scale);
Matrix3 ScaleMatrix3(float scaleX, float scaleY, float scaleZ);
Matrix3 ScaleMatrix3(float scale);
Matrix3 RotationMatrix3(float degree, const Vector3& rotation);
Matrix3 RotationMatrix3(const Vector3& rotation);
Matrix3 RotationMatrix3(float degreeX, float degreeY, float degreeZ);

Matrix3 TransformMatrix3(const Vector3& rotaion, const Vector3& scale);
// Matrix3 TransformMatrix3(const Quaternion& rotaion, const Vector3& scale);
Matrix3 TransformMatrix3(const Vector3& rotaion, float scale);
// Matrix3 TransformMatrix3(const Quaternion& rotaion, float scale);

Matrix4 ScaleMatrix4(const Vector3& scale);
Matrix4 ScaleMatrix4(float scaleX, float scaleY, float scaleZ);
Matrix4 ScaleMatrix4(float scale);
Matrix4 RotationMatrix4(float degree, const Vector3& rotation);
Matrix4 RotationMatrix4(const Vector3& rotation);
Matrix4 RotationMatrix4(float degreeX, float degreeY, float degreeZ);
Matrix4 TranslateMatrix4(const Vector3& translation);
Matrix4 TranslateMatrix4(float x, float y, float z);

Matrix4 TransformMatrix4(const Vector3& rotation, const Vector3& scale, const Vector3& translation);
// Matrix4 TransformMatrix4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale);
Matrix4 TransformMatrix4(const Vector3& rotation, float scale, const Vector3& translation);
// Matrix4 TransformMatrix4(const Vector3& translation, const Quaternion& rotation, float scale);

// FIXME
bool DecomposeSRT(const Matrix4& matrix, Vector3& scale, Vector3& rotation, Vector3& translation);

} // namespace Pt