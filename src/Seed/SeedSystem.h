#ifndef __SEEDSYSTEM_H__
#define __SEEDSYSTEM_H__

#include "Core/Component.h"
#include "Scripts/Behavior.h"
#include "Seed/Emitter.h"

namespace Kiwi::Seed {

class SeedSystem : public Behavior {
	K_COMPONENT_H(SeedSystem);
public:
	SeedSystem(Scene &s) : Behavior(s), emitters() {}

	void update(float dt) override;

	Component *clone() override;

	Vector<std::unique_ptr<Emitter>> emitters;
};

}

#endif // __SEEDSYSTEM_H__