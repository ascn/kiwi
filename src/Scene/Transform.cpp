#include "Scene/Transform.h"

namespace Kiwi {

K_COMPONENT_S(Transform)
Vector3 Transform::worldUp(0.f, 1.f, 0.f);
Vector3 Transform::worldRight(1.f, 0.f, 0.f);
Vector3 Transform::worldForward(0.f, 0.f, 1.f);

Transform::Transform() :
	position(0.0, 0.0, 0.0),
	scale(1.0, 1.0, 1.0),
	eulerAngles(0.0, 0.0, 0.0),
	transMat(),
	up(worldUp),
	right(worldRight),
	forward(worldForward)
{
	updateRotation();
	update();
}

Vector3 Transform::GetPosition() const { return position; }
Vector3 Transform::GetRotation() const { return eulerAngles; }
Vector3 Transform::GetScale() const { return scale; }

void Transform::SetPosition(Vector3 position) {
	this->position = position;
	update();
}

void Transform::SetRotation(Vector3 rotation) {
	this->eulerAngles = rotation;
	updateRotation();
	update();
}

void Transform::SetScale(Vector3 scale) {
	this->scale = scale;
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

Vector3 Transform::InverseTransformDirection(Vector3 direction) const {
	return Vector3();
}

Vector3 Transform::InverseTransformPoint(Vector3 point) const {
	return Vector3();
}

void Transform::LookAt(Vector3 target, Vector3 worldUp) {
	forward = glm::normalize(target - position);
	right = glm::abs(forward[1]) == 1 ? worldRight : glm::cross(worldUp, forward);
	up = glm::cross(forward, right);
	
}

void Transform::LookAt(Transform target, Vector3 worldUp) {
	LookAt(target.position, worldUp);
}

void Transform::update() {
	transMat = glm::translate(position) *
			   rotateMat *
			   glm::scale(scale);
}

void Transform::updateRotation() {
	rotateMat = glm::rotate(glm::radians(eulerAngles.x), Transform::right) *
				glm::rotate(glm::radians(eulerAngles.y), Transform::up) *
				glm::rotate(glm::radians(eulerAngles.z), Transform::forward);
	up = rotateMat * Vector4(worldUp, 0);
	right = rotateMat * Vector4(worldRight, 0);
	forward = rotateMat * Vector4(worldForward, 0);
}

void Transform::Rotate(Vector3 eulerAngles) {
	
}

void Transform::Translate(Vector3 translation) {
	position += translation;
	update();
}

}