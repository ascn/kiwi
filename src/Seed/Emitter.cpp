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

}