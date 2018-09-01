#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <glm.hpp>
#include <gtc/quaternion.hpp>

template <class T>
using Vector = std::vector<T>;
using String = std::string;
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix3 = glm::mat3;
using Matrix4 = glm::mat4;
using Quaternion = glm::quat;

#endif // __UTIL_H__