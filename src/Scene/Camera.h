#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "util.h"
#include "Core/Component.h"

#ifdef _WIN32
#undef near
#undef far
#endif

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
	float size;

	struct ClipPlanes {
		float near;
		float far;
	};

	ClipPlanes clip;

	int width;
	int height;

	Matrix4 &viewMatrix();
	void computeViewMatrix();
	Matrix4 &projectionMatrix();
	void computeProjectionMatrix();

	virtual Component *clone() {
		auto ret = new Camera();
		ret->projection = projection;
		ret->fov = fov;
		ret->size = size;
		ret->clip = clip;
		ret->width = width;
		ret->height = height;
		ret->viewMat = viewMat;
		ret->projMat = projMat;
		return ret;
	}

private:
	Matrix4 viewMat;
	Matrix4 projMat;
};

}

#endif // __CAMERA_H__