#ifndef __GRAVITYMODULE_H__
#define __GRAVITYMODULE_H__

#include "Seed/Module.h"
#include "Seed/ParticleManager.h"

namespace Kiwi::Seed {

class GravityModule : public Module {
public:
	GravityModule(ParticleManager &pm) : Module(pm) {}

	virtual void update(float dt) override {
		for (int i = 0; i < particles.maxParticles; ++i) {
			particles.setForce(i, Vector3(0, -9.8, 0));
		}
	}
};

}

#endif // __GRAVITYMODULE_H__