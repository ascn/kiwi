#include "Seed/Emitter.h"
#include "Seed/SeedSystem.h"
#include "Seed/FountainSpawner.h"
#include "Scene/Transform.h"

#include "Seed/SeedMath.h"

namespace Kiwi::Seed {

FountainSpawner::FountainSpawner(Emitter *e) :
	Spawner(e),
	angle(glm::radians(30.f)),
	axis(Vector3(0, 1, 0)),
	minSpeed(1),
	maxSpeed(5),
	lifetime(30)
{}

const std::unordered_map<String, std::any> &FountainSpawner::GetSpawnedProperties() {
	ComputeSpawnedProperties();
	return nextSpawnedProperties;
}

void FountainSpawner::ComputeSpawnedProperties() {
	Transform *transform = emitter->system->gameObject->GetComponent<Transform>();
	float cosTheta = glm::cos(angle);
	float z = Math::randRange(cosTheta, 1);
	float phi = Math::randRange(0, 2 * Math::PI);
	float fact = glm::sqrt(1 - (z * z));
	Vector3 normDir(fact * glm::cos(phi), z, fact * glm::sin(phi));
	if (axis.y < 0) { normDir *= -1.f; }
	if (axis != Transform::worldUp) {
		// Transform direction into emitter's local space
		Vector3 rotAxis = glm::cross(axis, Transform::worldUp);
		float rotAngle = glm::acos(glm::dot(axis, Transform::worldUp));
		Matrix4 rot = glm::rotate(rotAngle, rotAxis);
		normDir = rot * Vector4(normDir, 0);
	}
	normDir = glm::normalize(normDir);
	
	// Get speed
	float speed = Math::randRange(minSpeed, maxSpeed);

	// Compute all spawned properties
	Vector3 spawnedPosition = transform->GetPosition();
	Vector3 spawnedVelocity = normDir * speed;
	Vector3 spawnedAcceleration = Vector3(0);
	Vector3 spawnedForce = Vector3(0);
	float spawnedMass = 1.f;
	Vector4 spawnedColor = Vector4(1);
	float spawnedLifetime = lifetime;

	// Assign nextSpawnedProperties
	nextSpawnedProperties[PropertyNames::Position] = spawnedPosition;
	nextSpawnedProperties[PropertyNames::Velocity] = spawnedVelocity;
	nextSpawnedProperties[PropertyNames::Acceleration] = spawnedAcceleration;
	nextSpawnedProperties[PropertyNames::Force] = spawnedForce;
	nextSpawnedProperties[PropertyNames::Mass] = spawnedMass;
	nextSpawnedProperties[PropertyNames::Color] = spawnedColor;
	nextSpawnedProperties[PropertyNames::Lifetime] = spawnedLifetime;
}

}