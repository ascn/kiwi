#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Core/Component.h"
#include "Rendering/Material.h"
#include "Scene/Scene.h"

namespace Kiwi {

class Renderer;
using RendererList = std::vector<Renderer *>;

/**
 * Base class for all renderers.
 *
 * A renderer is a Component attached to a GameObject that makes
 * it appear on screen. An instance of Renderer can be toggled
 * (#enabled), and its material can be accessed through #material.
 *
 * See MeshRenderer.
 */
class Renderer : public Component {
public:
	Renderer();
	Renderer(Scene &s);
	virtual ~Renderer() {}
	virtual void render(bool useMaterial = true) = 0;

	virtual Component *clone() = 0;

	static RendererList &getRendererList() {
		static RendererList rendererList;
		return rendererList;
	}

	bool enabled;
	Material *material;
	Scene *scene;
};

}

#endif // __RENDERER_H__