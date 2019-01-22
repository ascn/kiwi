#include "Rendering/Renderer.h"
#include "Scene/Scene.h"

namespace Kiwi {

Renderer::Renderer() :
	enabled(true),
	material(nullptr),
	scene(nullptr) {}

Renderer::Renderer(Scene &s) :
	enabled(true),
	material(nullptr),
	scene(&s)
{
	s.renderList.push_back(this);
}

}