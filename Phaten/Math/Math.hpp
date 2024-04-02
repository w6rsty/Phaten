#pragma once

#include <cstdlib>
#include <cmath>
#include <limits>

#undef M_PI
#undef M_2_PI

static const float M_PI = 3.14159265358979323846f;
static const float M_2_PI = 6.28318530717958647692f;
static const float M_DEG_TO_RAD = M_PI / 180.0f;
static const float M_RAD_TO_DEG = 180.0f / M_PI;
static const float M_EPSILON = 0.000000001f;

static bool EpsilonEqual(float lhs, float rhs, float epsilon = M_EPSILON) { return lhs + epsilon >= rhs && lhs - epsilon <= rhs; }
static bool EpsilonNotEqual(float lhs, float rhs, float epsilon = M_EPSILON) { return lhs + epsilon < rhs || lhs - epsilon > rhs; }
static bool IsNaN(float value) { return value != value; }
static float Abs(float value) { return value < 0.0f ? -value : value; }

static float Radians(float degrees) { return degrees * M_DEG_TO_RAD; }
static float Degrees(float radians) { return radians * M_RAD_TO_DEG; }