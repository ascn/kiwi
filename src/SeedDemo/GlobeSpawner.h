#ifndef __GLOBE_SPAWNER_H__
#define __GLOBE_SPAWNER_H__

#include "Seed/Spawner.h"

class GlobeSpawner : public Kiwi::Seed::Spawner {
public:
	GlobeSpawner(Kiwi::Seed::Emitter *e);

	virtual ~GlobeSpawner() {}

	float radius;
	float minSpeed;
	float maxSpeed;

protected:
	virtual void ComputeSpawnedProperties() override;
};

#endif // __GLOBE_SPAWNER_H__