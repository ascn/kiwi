#ifndef __ORBITINGOBJECT_H__
#define __ORBITINGOBJECT_H__

#include "Core/GameObject.h"
#include "Scripts/Behavior.h"

class OrbitingBehavior : public Kiwi::Behavior {
	K_COMPONENT_H(OrbitingBehavior);
public:
	OrbitingBehavior(Kiwi::Scene &s_, Vector3 center_, float radius_) :
		Behavior(s),
		center(center_),
		radius(radius_),
		currentAngle(0)
	{}
	~OrbitingBehavior() {}

	void update() override {

		GetComponent<Kiwi::Transform>().LookAt(center, Transform::worldUp);
		currentAngle += 1;
		if (currentAngle >= 360) { currentAngle = 0; }
	}

	Kiwi::Component *clone() override {
		return new OrbitingBehavior(*s, center, radius);
	}

	Vector3 center;
	float radius;

private:
	float currentAngle;
}

class OrbitingObject : public Kiwi::GameObject {

}

#endif // __ORBITINGOBJECT_H__