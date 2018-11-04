#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include "Renderer.h"

namespace Kiwi {

/**
 * A Renderer that renders the Mesh contained in a MeshFilter
 * Component using the Material contained and using the Transform
 * attached to the GameObject.
 *
 * If the MeshFilter or Transform components don't exist on the
 * GameObject this MeshRenderer is attached to, rendering fails.
 */
class MeshRenderer : public Renderer {
public:
K_COMPONENT_H(MeshRenderer)

	MeshRenderer();
	virtual ~MeshRenderer() override;
	virtual void render() override;

	virtual Component *clone() override {
		auto *ret = new MeshRenderer();
		Renderer::getRendererList().push_back(ret);
		return ret;
	}
};

}

#endif // __MESH_RENDERER_H__