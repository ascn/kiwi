#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <GL/glew.h>

#include "Core/Object.h"
#include "Rendering/Texture.h"

namespace Kiwi {

class Framebuffer : public Object {
public:
	Framebuffer(const String &name, int width, int height);
	~Framebuffer();

	void attachNewColorBuffer();
	void attachNewDepthBuffer();
	void attachNewStencilBuffer();

private:
	friend class Light;
	friend class Scene;
	int width;
	int height;
	GLuint FBO;
	GLuint color, depth, stencil;
};

}

#endif // __FRAMEBUFFER_H__