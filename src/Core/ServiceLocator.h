#ifndef __SERVICE_LOCATOR_H__
#define __SERVICE_LOCATOR_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "Logger.h"
#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"

namespace Kiwi {

class ServiceLocator {
private:
	ServiceLocator() = delete;
	~ServiceLocator() = delete;

public:
	static void initLoggers();
	static Logger getLogger(const String &logger = LOG_DEFAULT);

	static MeshLibrary *getMeshLibrary();
	static TextureLibrary *getTextureLibrary();

	static void initGLFW();
	static void terminateGLFW();

	// must be called after getMainWindow
	static void initGLEW();
	static GLFWwindow *getMainWindow();

	void glfwErrorCallback(int error, const char *desc);

	static int width, height;

};

}

#endif // __SERVICE_LOCATOR_H__
