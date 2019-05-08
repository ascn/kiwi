#ifndef __MODULE_H__
#define __MODULE_H__

#include "Core/Object.h"
#include "Seed/ParticleManager.h"

namespace Kiwi::Seed {

class Module : public Object {
public:
	Module(ParticleManager &pm) : particles(pm), enabled(true) {}
	virtual ~Module() {};

	virtual void update(float dt) = 0;

	void setEnabled(bool enabled_) {
		enabled = enabled_;
	}

	bool isEnabled() { return enabled; }

protected:
	ParticleManager &particles;
	bool enabled;
};

}

#endif // __MODULE_H__