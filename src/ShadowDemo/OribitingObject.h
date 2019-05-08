#ifndef __ORBITINGOBJECT_H__
#define __ORBITINGOBJECT_H__

#include "Core/GameObject.h"
#include "Scripts/Behavior.h"

class OrbitingBehavior : public Kiwi::Behavior {
	K_COMPONENT_H(OrbitingBehavior);
public:
	OrbitingBehavior(Kiwi::Scene &s_, Vector3 center_, float radius_) :
		Kiwi::Behavior(s_),
		center(center_),
		radius(radius_),
		currentAngle(0)
	{}
	~OrbitingBehavior() {}

	void update(float dt) override {
		// Get position as if this was a unit circle
		float x = glm::cos(glm::radians(currentAngle));
		float y = glm::sin(glm::radians(currentAngle));
		x *= radius;
		y *= radius;
		float transY = gameObject->GetComponent<Kiwi::Transform>()->GetPosition().y;
		Vector3 pos(x, transY, y);
		pos += center;
		gameObject->GetComponent<Kiwi::Transform>()->SetPosition(pos);
		gameObject->GetComponent<Kiwi::Transform>()->LookAt(center, Transform::worldUp);
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
};

class OrbitingObject : public Kiwi::GameObject {
public:
	OrbitingObject(const String &name, Vector3 center_, float radius_, Scene *scene) :
		Kiwi::GameObject(name)
	{
		AddComponent<OrbitingBehavior>(*scene, center_, radius_);
	}
};

#endif // __ORBITINGOBJECT_H__