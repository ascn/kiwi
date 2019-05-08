#ifndef __TICK_LIFETIME_MODULE_H__
#define __TICK_LIFETIME_MODULE_H__

#include "Seed/Module.h"
#include "Seed/ParticleManager.h"

namespace Kiwi::Seed {

class TickLifetimeModule : public Module {
public:
	TickLifetimeModule(ParticleManager &pm) : Module(pm) {}

	virtual void update(float dt) override {
		for (unsigned int i = 0; i < particles.maxParticles; ++i) {
			if (particles.isParticleValid(i)) {
				float lifetime = particles.getLifetime(i);
				if (lifetime > 0) {
					particles.setLifetime(i, lifetime - dt);
				}
				if (lifetime - dt < 0) {
					particles.despawnParticle(i);
				}
			}
		}
	}
};

}

#endif // __TICK_LIFETIME_MODULE_H__