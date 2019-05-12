#include "GlobeModule.h"

using namespace Kiwi;
using namespace Kiwi::Seed;

GlobeModule::GlobeModule(ParticleManager &pm) :
	Module(pm),
	radius(1),
	center(Vector3(0))
{}

void GlobeModule::update(float dt) {
	for (int i = 0; i < particles.maxParticles; ++i) {
		Vector3 v = particles.getVelocity(i);
		float m = particles.getMass(i);
		float tanSpeed = glm::length(v);
		Vector3 dir = center - particles.getPosition(i);
		float forceMag = (m * tanSpeed * tanSpeed) / radius;
		particles.setForce(i, forceMag * dir);
	}
}