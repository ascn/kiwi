#ifndef __ROTATINGMESH_H__
#define __ROTATINGMESH_H__

#include "Core/GameObject.h"
#include "Scripts/Behavior.h"

using namespace Kiwi;

class RotatingBehavior : public Behavior {
	K_COMPONENT_H(RotatingBehavior);
public:
	RotatingBehavior(Scene &s) : Behavior(s), growing(true) {}
	~RotatingBehavior() {}

	void update(float dt) override {
		auto transform = Component::gameObject->GetComponent<Transform>();
		Vector3 scale = transform->GetScale();
		if (scale.x < 2 && growing) {
			transform->SetScale(scale + Vector3(0.01, 0, 0));
		} else if (scale.x > 1 && !growing) {
			transform->SetScale(scale - Vector3(0.01, 0, 0));
		}
		if (scale.x >= 2 && growing) { growing = !growing; }
		if (scale.x <= 1 && !growing) { growing = !growing; }
	}

	Component *clone() override {
		return new RotatingBehavior(*s);
	}

	bool growing;
};

class RotatingMesh : public GameObject {
public:
	RotatingMesh(const String &name, Mesh *mesh, Material *material, Scene *scene) :
		GameObject(name)
	{
		AddComponent<Kiwi::MeshFilter>(mesh);
		AddComponent<Kiwi::MeshRenderer>(*scene).material = material;
		AddComponent<RotatingBehavior>(*scene);
	}
};

#endif // __ROTATINGMESH_H__