#include "Seed/SeedSystem.h"

namespace Kiwi::Seed {

K_COMPONENT_S(SeedSystem)

void SeedSystem::update(float dt) {
	for (const auto &emitter : emitters) {
		emitter->update(dt);
	}
}

Component *SeedSystem::clone() {
	return nullptr;
}

}