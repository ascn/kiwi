#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/transform.hpp>
#include "util.h"
#include "Core/Component.h"

namespace Kiwi {

/**
 * Defines the position, rotation, and scale of a GameObject.
 *
 * Every GameObject is automatically created with a Transform
 * component.
 */
class Transform : public Component {
public:
K_COMPONENT_H(Transform)
	Transform();
	virtual ~Transform() override {}

	Vector3 GetPosition() const;
	void SetPosition(Vector3 position);

	Vector3 GetRotation() const;
	void SetRotation(Vector3 rotation);

	Vector3 GetScale() const;
	void SetScale(Vector3 scale);


	Vector3 TransformDirection(Vector3 direction) const;
	Vector3 TransformPoint(Vector3 point) const;
	Vector3 InverseTransformDirection(Vector3 direction) const;
	Vector3 InverseTransformPoint(Vector3 point) const;
	void LookAt(Vector3 target, Vector3 worldUp);
	void LookAt(Transform target, Vector3 worldUp);
	void Rotate(Vector3 eulerAngles);
	void Translate(Vector3 translation);

	/**
	 * Clones the position, rotation, scale, and transformation
	 * matrix of this component.
	 */
	virtual Component *clone() override {
		auto *ret = new Transform();
		ret->position = position;
		ret->eulerAngles = eulerAngles;
		ret->scale = scale;
		ret->transMat = transMat;
		return ret;
	}

	static Vector3 worldUp;
	static Vector3 worldRight;
	static Vector3 worldForward;

private:
	void update();
	void updateRotation();

	Vector3 position;
	Vector3 eulerAngles;
	Vector3 scale;
	Matrix4 transMat;
	Matrix4 rotateMat;

	Vector3 up;
	Vector3 right;
	Vector3 forward;
};

}

#endif // __TRANSFORM_H__
