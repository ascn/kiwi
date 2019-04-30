#include "Rendering/Renderer.h"
#include "Scene/Scene.h"

namespace Kiwi {

Renderer::Renderer() :
	enabled(true),
	material(nullptr),
	scene(nullptr),
	castShadows(true)
{}

Renderer::Renderer(Scene &s) :
	enabled(true),
	material(nullptr),
	scene(&s),
	castShadows(true)
{
	s.renderList.push_back(this);
}

}