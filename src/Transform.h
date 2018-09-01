#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/transform.hpp>
#include "util.h"
#include "Component.h"

namespace Kiwi {

class Transform : public Component {
public:
	K_COMPONENT_H(Transform)
	Transform();
	~Transform();

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
	void LookAt(Transform target, Vector3 worldUp);
	void Rotate(Vector3 eulerAngles);
	void Translate(Vector3 translation);

	virtual Component *clone() override;

private:
	void update();

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	Vector3 eulerAngles;
	Matrix4 transMat;

	static Vector3 up;
	static Vector3 right;
	static Vector3 forward;
};

}

#endif // __TRANSFORM_H__
