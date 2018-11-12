#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

template <class T>
using Vector = std::vector<T>;
using String = std::string;

// Typedefs for glm types
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using DVector2 = glm::dvec2;
using DVector3 = glm::dvec3;
using DVector4 = glm::dvec4;
using IVector2 = glm::ivec2;
using IVector3 = glm::ivec3;
using IVector4 = glm::ivec4;
using UVector2 = glm::uvec2;
using UVector3 = glm::uvec3;
using UVector4 = glm::uvec4;
using BVector2 = glm::bvec2;
using BVector3 = glm::bvec3;
using BVector4 = glm::bvec4;
using Matrix2 = glm::mat2;
using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;
using Quaternion = glm::quat;

#endif // __UTIL_H__