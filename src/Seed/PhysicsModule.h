#ifndef __PHYSICSMODULE_H__
#define __PHYSICSMODULE_H__

#include "Seed/Module.h"
#include "Seed/ParticleManager.h"

namespace Kiwi::Seed {

class PhysicsModule : public Module {
public:
	PhysicsModule(ParticleManager &pm) : Module(pm) {}

	virtual void update(float dt) override {
		// Integrate
		for (unsigned int i = 0; i < particles.maxParticles; ++i) {
			if (particles.isParticleValid(i)) {
				Vector3 newAcc = particles.getForce(i) / particles.getMass(i);
				particles.setAcceleration(i, newAcc);
				Vector3 newVel = particles.getVelocity(i) + newAcc * dt;
				Vector3 newPos = particles.getPosition(i) + newVel * dt;
				particles.setVelocity(i, newVel);
				particles.setPosition(i, newPos);
			}
		}
	}
};

}

#endif // __PHYSICSMODULE_H__