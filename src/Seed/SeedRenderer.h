#ifndef __SEED_RENDERER_H__
#define __SEED_RENDERER_H__

#include "Rendering/Renderer.h"

namespace Kiwi::Seed {

class Emitter;

class SeedRenderer : public Renderer {
public:
	SeedRenderer(Scene &s, Emitter *emitter_) :
		Renderer(s),
		emitter(emitter_)
	{}

	virtual ~SeedRenderer() override {};

	virtual void render(bool useMaterial = true) override = 0;

	virtual Component *clone() = 0;

	Emitter *emitter;
};

}

#endif // __SEED_RENDERER_H__