#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/transform.hpp>
#include "util.h"
#include "Core/GameObject.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"

namespace Kiwi {

K_COMPONENT_S(Camera)
Camera::Camera() :
	projection(Camera::Projection::Perspective),
	fov(45.0),
	clip({ 0.01, 100 }),
	width(1280),
	height(720),
	viewMat(0.f),
	projMat(0.f) {}

Camera::~Camera() {}

Matrix4 &Camera::viewMatrix() {
	return viewMat;
}

void Camera::computeViewMatrix() {
	viewMat = Component::gameObject->GetComponent<Transform>()->GetViewMatrix();
}

Matrix4 &Camera::projectionMatrix() {
	return projMat;
}

void Camera::computeProjectionMatrix() {
	switch (Camera::projection) {
	case Camera::Projection::Perspective:
	{
		projMat = glm::perspective(glm::radians(fov),
								   static_cast<float>(width) / static_cast<float>(height),
								   clip.near,
								   clip.far);
		break;
	}
	case Camera::Projection::Orthographic:
	{
		float aspect = static_cast<float>(width) / static_cast<float>(height);
		projMat = glm::ortho(-size * aspect, size * aspect,
							 -size / aspect, size / aspect,
							 0.f, clip.far);
		break;
	}
	default:
		break;
	}
}

}