#ifndef __FOUNTAIN_SPAWNER_H__
#define __FOUNTAIN_SPAWNER_H__

#include "Seed/Spawner.h"

namespace Kiwi::Seed {

/**
 * Spawns particles at the emitter's position with a velocity with direction
 * uniformly sampled in a cone formed by an axis and angle around it and
 * magnitude uniformly distributed between two values.
 */
class FountainSpawner : public Spawner {
public:
	FountainSpawner(Emitter *e);

	virtual ~FountainSpawner() {}

	float angle;
	Vector3 axis;
	float minSpeed;
	float maxSpeed;
	
	float lifetime;

protected:
	virtual void ComputeSpawnedProperties() override;
};

}

#endif // __FOUNTAIN_SPAWNER_H__