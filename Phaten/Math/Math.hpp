#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>

#undef M_PI
static const float M_PI = 3.14159265358979323846f;
static const float M_EPSILON = 0.000001f;

static bool Equal(float lhs, float rhs, float epsilon = M_EPSILON) { return lhs + epsilon >= rhs && lhs - epsilon <= rhs; }
static bool IsNaN(float value) { return value != value; }
static float Abs(float value) { return value < 0.0f ? -value : value; }