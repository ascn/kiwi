#ifndef __SEED_POINT_RENDERER_H__
#define __SEED_POINT_RENDERER_H__

#include <GL/glew.h>
#include "Seed/SeedRenderer.h"

namespace Kiwi::Seed {

class SeedPointRenderer : public SeedRenderer { 
K_COMPONENT_H(SeedPointRenderer)
public:
	SeedPointRenderer(Scene &s, Emitter *emitter);

	virtual ~SeedPointRenderer() override;

	virtual void render(bool useMaterial = true) override;

	virtual Component *clone() {
		return nullptr;
	}

private:
	GLuint VBO;
	GLuint VAO;

	struct PointRenderData {
		Vector3 position;
		Vector4 color;
	};

	Vector<PointRenderData> dataVec;
};

}

#endif // __SEED_POINT_RENDERER_H__