#include "Seed/Emitter.h"
#include "Seed/SeedSystem.h"
#include "Scene/Transform.h"
#include "Seed/SeedMath.h"

#include "GlobeSpawner.h"

using namespace Kiwi;
using namespace Kiwi::Seed;

GlobeSpawner::GlobeSpawner(Emitter *e) :
	Spawner(e),
	radius(5.f),
	minSpeed(1.f),
	maxSpeed(5.f)
{}

void GlobeSpawner::ComputeSpawnedProperties() {
	Transform *transform = emitter->system->gameObject->GetComponent<Transform>();

	Vector3 sphereCenter = transform->GetPosition();
	// First find a spawn point (random point on sphere centered at sphereCenter
	// and with radius radius
	Vector2 sample(Math::randRange(0, 1), Math::randRange(0, 1));
	float z = 1.f - 2.f * sample.x;
	float r = glm::sqrt(glm::max(0.f, 1.f - z * z));
	float phi = 2.f * Math::PI * sample.y;
	Vector3 unitSphereSample(r * glm::cos(phi), r * glm::sin(phi), z);

	// Compute velocity
	Vector3 direction = glm::normalize(glm::cross(unitSphereSample, Transform::worldUp));
	float speed = Math::randRange(minSpeed, maxSpeed);

	// Compute all spawned properties
	Vector3 spawnedPosition = sphereCenter + (unitSphereSample * radius);
	Vector3 spawnedVelocity = direction * speed;
	Vector3 spawnedAcceleration = Vector3(0);
	Vector3 spawnedForce = Vector3(0);
	float spawnedMass = 1.f;
	Vector4 spawnedColor = Vector4(1);
	float spawnedLifetime = 0;

	// Assign nextSpawnedProperties
	nextSpawnedProperties[PropertyNames::Position] = spawnedPosition;
	nextSpawnedProperties[PropertyNames::Velocity] = spawnedVelocity;
	nextSpawnedProperties[PropertyNames::Acceleration] = spawnedAcceleration;
	nextSpawnedProperties[PropertyNames::Force] = spawnedForce;
	nextSpawnedProperties[PropertyNames::Mass] = spawnedMass;
	nextSpawnedProperties[PropertyNames::Color] = spawnedColor;
	nextSpawnedProperties[PropertyNames::Lifetime] = spawnedLifetime;
}