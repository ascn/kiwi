#ifndef __SEEDMATH_H__
#define __SEEDMATH_H__

#include <gtc/random.hpp>

namespace Kiwi::Seed::Math {

constexpr float PI = 3.14159265359f;

float randRange(float min, float max) {
	return glm::linearRand(min, max);
}

}

#endif // __SEEDMATH_H__