#ifndef __EMITTER_H__
#define __EMITTER_H__

#include "Core/Object.h"
#include "Seed/Module.h"
#include "Seed/PhysicsModule.h"
#include "Seed/Spawner.h"
#include "Seed/SeedRenderer.h"
#include "Scene/Scene.h"

#include <functional>

namespace Kiwi::Seed {

class SeedSystem;

class Emitter : public Object {
public:
	/**
	 * Construct an emitter that can have up to maxParticles_
	 * particles spawned at any particular moment and spawns
	 * spawnRate_ particles per second.
	 */
	Emitter(SeedSystem *system_, unsigned int maxParticles_, float spawnRate_);

	/**
	 * Construct an emitter that can have up to maxParticles_
	 * particles spawned at any particular moment. spawnRate_
	 * is a function that takes in the elapsed time of this
	 * emitter and outputs the spawn rate (number of particles
	 * spawned per second.)
	 */
	Emitter(SeedSystem *system_, unsigned int maxParticles_, std::function<float(float)> spawnRate_);

	virtual ~Emitter() {};

	virtual void update(float dt) {
		elapsedTime += dt;
		for (const auto &module : modules) {
			module->update(dt);
		}

		// Check if we should spawn particle
		if (timeSinceLastSpawn > timeBetweenSpawns) {
			timeBetweenSpawns = 1.f / spawnRate(elapsedTime);
			timeSinceLastSpawn = 0.f;
			int particle = manager.spawnParticle();
			if (particle >= 0) {
				// Particle is valid, set properties
				const auto &spawnProps = spawner->GetSpawnedProperties();
				for (const auto &prop : spawnProps) {
					manager.set(prop.first, particle, prop.second);
				}
				manager.setParticleValid(particle, true);
			}
		} else {
			timeSinceLastSpawn += dt;
		}
	}

	template <typename T>
	T &AddModule();

	template <typename T>
	T &AddRenderer(Scene &s);

	template <typename T>
	T &SetSpawner();

public:
	bool enabled;
	ParticleManager manager;
	SeedSystem *system;

private:
	Vector<std::unique_ptr<Module>> modules;
	Vector<std::unique_ptr<SeedRenderer>> renderers;
	std::unique_ptr<Spawner> spawner;
	float elapsedTime;
	std::function<float(float)> spawnRate;
	float timeBetweenSpawns;
	float timeSinceLastSpawn;
};

template <typename T>
T &Emitter::AddModule() {
	static_assert(std::is_base_of_v<Module, T>, "can't add a non Module type!");
	auto ptr = std::make_unique<T>(manager);
	auto ret = ptr.get();
	modules.push_back(std::move(ptr));
	return *ret;
}

template <typename T>
T &Emitter::AddRenderer(Scene &s) {
	static_assert(std::is_base_of_v<SeedRenderer, T>, "can't add a non SeedRenderer type!");
	auto ptr = std::make_unique<T>(std::forward<Scene &>(s), this);
	auto ret = ptr.get();
	renderers.push_back(std::move(ptr));
	ret->gameObject = system->gameObject;
	return *ret;
}

template <typename T>
T &Emitter::SetSpawner() {
	static_assert(std::is_base_of_v<Spawner, T>, "can't add a non Spawner type!");
	auto derivedSpawner = std::make_unique<T>(this);
	auto ret = derivedSpawner.get();
	spawner = std::move(derivedSpawner);
	return *ret;
}

}

#endif // __EMITTER_H__