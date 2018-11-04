#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "util.h"
#include "Component.h"

namespace Kiwi {

class Camera : public Component {
public:
K_COMPONENT_H(Camera)

	Camera();
	~Camera();

	enum class Projection : char {
		Perspective,
		Orthographic
	} projection;

	float fov;

	struct clipPlanes {
		float near;
		float far;
	};

	clipPlanes clip;
};

}

#endif // __CAMERA_H__