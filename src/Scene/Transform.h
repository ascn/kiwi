#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/transform.hpp>
#include "util.h"
#include "Core/Component.h"

/** @namespace Kiwi
 * @brief Class @ref Transform
 */
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

	Matrix4 GetViewMatrix() const;

	/**
	 * Transforms the vector direction from local space to world space.
	 *
	 * The transformation operation does not use the position or scale
	 * of this Transform.
	 * 
	 * @param  direction Vector3 direction to transform
	 * @return           transformed direction
	 */
	Vector3 TransformDirection(Vector3 direction) const;

	/**
	 * Transforms the vector point from local space to world space.
	 *
	 * The scale and translation of this transform affects the
	 * transformation operation.
	 * 
	 * @param  point Vector3 point to transform
	 * @return       transformed point
	 */
	Vector3 TransformPoint(Vector3 point) const;

	/**
	 * Transforms the vector direction from world space to local space.
	 *
	 * The transformation operation does not use the position or scale
	 * of this Transform.
	 * 
	 * @param  direction Vector3 direction to tranasform
	 * @return           transformed direction
	 */
	Vector3 InverseTransformDirection(Vector3 direction) const;

	/**
	 * Transforms the vector point from world space to local space.
	 *
	 * The scale and translation of this transform affects the
	 * transformation operation.
	 * 
	 * @param  point Vector3 point to transform
	 * @return       transformed point
	 */
	Vector3 InverseTransformPoint(Vector3 point) const;

	/**
	 * Rotates the transform so that this forward vector points at
	 * <code>target</code>.
	 *
	 * Then rotates the transform so that its up vector points at
	 * the vector specified by <code>worldUp</code>.
	 * @param target  position to look at
	 * @param worldUp vector specifying upward direction
	 */
	void LookAt(Vector3 target, Vector3 worldUp = Transform::worldUp);

	/**
	 * Rotates the transform so that this forward vector points at
	 * the location <code>target</code>.
	 *
	 * Then rotates the transform so that its up vector points at
	 * the vector specified by <code>worldUp</code>.
	 * @param target  transform to look at
	 * @param worldUp vector specifying upward direction
	 */
	void LookAt(Transform target, Vector3 worldUp = Transform::worldUp);
	void Rotate(Vector3 eulerAngles);
	void RotateAround(Vector3 point, Vector3 axis, float angle);
	void Translate(Vector3 translation);

	/**
	 * Clones the position, rotation, scale, and transformation
	 * matrix of this component.
	 */
	virtual Component *clone() override {
		auto *ret = new Transform();
		ret->position = position;
		ret->rotation = rotation;
		ret->eulerAngles = eulerAngles;
		ret->scale = scale;
		ret->transMat = transMat;
		return ret;
	}

	/**
	 * Shorthand for Vector3(0, 1, 0)
	 */
	static Vector3 worldUp;

	/**
	 * Shorthand for Vector3(1, 0, 0)
	 */
	static Vector3 worldRight;

	/**
	 * Shorthand for Vector3(0, 0, 1)
	 */
	static Vector3 worldForward;

private:
	void update();
	void updateRotationMatrix();
	void updateEulerFromQuat();
	void updateQuatFromEuler();

	Vector3 position;
	Quaternion rotation;
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
