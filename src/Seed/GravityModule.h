#ifndef __GRAVITYMODULE_H__
#define __GRAVITYMODULE_H__

#include "Seed/Module.h"
#include "Seed/ParticleManager.h"

namespace Kiwi::Seed {

class GravityModule : public Module {
public:
	GravityModule(ParticleManager &pm);

	virtual void update(float dt) override;

	Vector3 gravity;
};

}

#endif // __GRAVITYMODULE_H__