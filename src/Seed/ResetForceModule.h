#ifndef __RESETFORCEMODULE_H__
#define __RESETFORCEMODULE_H__

#include "Seed/Module.h"
#include "Seed/ParticleManager.h"

namespace Kiwi::Seed {

class ResetForceModule : public Module {
public:
	ResetForceModule(ParticleManager &pm) : Module(pm) {}

	virtual void update(float dt) override {
		for (int i = 0; i < particles.maxParticles; ++i) {
			particles.setForce(i, Vector3(0));
		}
	}
};

}

#endif // __RESETFORCEMODULE_H__