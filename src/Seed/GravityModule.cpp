#include "Seed/GravityModule.h"

namespace Kiwi::Seed {

GravityModule::GravityModule(ParticleManager &pm) :
	Module(pm),
	gravity(Vector3(0, -9.8, 0))
{}

void GravityModule::update(float dt) {
	for (int i = 0; i < particles.maxParticles; ++i) {
		particles.setForce(i, gravity);
	}
}

}