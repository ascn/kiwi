#ifndef __GLOBE_MODULE_H__
#define __GLOBE_MODULE_H__

#include "Seed/Module.h"
#include "Seed/ParticleManager.h"

class GlobeModule : public Kiwi::Seed::Module {
public:
	GlobeModule(Kiwi::Seed::ParticleManager &pm);

	virtual void update(float dt) override;

	float radius;
	Vector3 center;
};

#endif // __GLOBE_MODULE_H__