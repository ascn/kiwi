#include "Seed/Emitter.h"

namespace Kiwi::Seed {

Emitter::Emitter(SeedSystem *system_, unsigned int maxParticles_, float spawnRate_) :
	Emitter(system_, maxParticles_, [spawnRate_](float) { return spawnRate_; })
{}

Emitter::Emitter(SeedSystem *system_, unsigned int maxParticles_, std::function<float(float)> spawnRate_) :
	enabled(true),
	manager(maxParticles_),
	system(system_),
	modules(),
	renderers(),
	elapsedTime(0),
	spawnRate(spawnRate_),
	timeBetweenSpawns(0.f),
	timeSinceLastSpawn(0.f)
{}

void Emitter::update(float dt) {
	elapsedTime += dt;
	for (const auto &module : modules) {
		if (module->isEnabled()) {
			module->update(dt);
		}
	}
	timeBetweenSpawns = 1.f / spawnRate(elapsedTime);

	// Check if we should spawn particle
	if (timeSinceLastSpawn > timeBetweenSpawns) {
		int numParticles = static_cast<int>(glm::floor(timeSinceLastSpawn / timeBetweenSpawns));
		timeSinceLastSpawn = 0.f;
		for (int i = 0; i < numParticles; ++i) {
			int particle = manager.spawnParticle();
			if (particle >= 0) {
				// Particle is valid, set properties
				const auto &spawnProps = spawner->GetSpawnedProperties();
				for (const auto &prop : spawnProps) {
					manager.set(prop.first, particle, prop.second);
				}
				manager.setParticleValid(particle, true);
			}
		}
	} else {
		timeSinceLastSpawn += dt;
	}
}

}