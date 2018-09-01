#include "Transform.h"

namespace Kiwi {

K_COMPONENT_S(Transform, "Transform")
Vector3 Transform::up(0.f, 1.f, 0.f);
Vector3 Transform::right(1.f, 0.f, 0.f);
Vector3 Transform::forward(0.f, 0.f, 1.f);

Transform::Transform() :
	position(0.0, 0.0, 0.0),
	rotation(0.0, 0.0, 0.0, 0.0),
	scale(1.0, 1.0, 1.0),
	eulerAngles(0.0, 0.0, 0.0),
	transMat()
	{
	update();
	}
Transform::~Transform() {}

Vector3 Transform::GetPosition() const { return position; }
Vector3 Transform::GetRotation() const { return eulerAngles; }
Vector3 Transform::GetScale() const { return scale; }

void Transform::SetPosition(Vector3 position) {
	this->position = position;
	update();
}

Vector3 Transform::TransformDirection(Vector3 direction) const {
	Vector4 transDir = transMat * Vector4(direction, 0);
	return Vector3(transDir.x, transDir.y, transDir.z);
}

Vector3 Transform::TransformPoint(Vector3 point) const {
	Vector4 transPt = transMat * Vector4(point, 1);
	return Vector3(transPt.x, transPt.y, transPt.z);
}

void Transform::update() {
	transMat = glm::translate(position) *
			   glm::rotate(eulerAngles.x, Transform::right) *
			   glm::rotate(eulerAngles.y, Transform::up) *
			   glm::rotate(eulerAngles.z, Transform::forward) *
			   glm::scale(scale);
}

Component *Transform::clone() {
	auto *ret = new Transform();
	ret->position = position;
	ret->rotation = rotation;
	ret->scale = scale;
	ret->eulerAngles = eulerAngles;
	ret->transMat = transMat;
	return ret;
}

}