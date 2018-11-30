#include "Rendering/Renderer.h"

namespace Kiwi {

Renderer::Renderer() {
	Renderer::getRendererList().push_back(this);
}

}