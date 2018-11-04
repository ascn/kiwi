#ifndef __SERVICE_LOCATOR_H__
#define __SERVICE_LOCATOR_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "Logger.h"
#include "Rendering/MeshLibrary.h"

namespace Kiwi {

class ServiceLocator {
private:
	ServiceLocator() = delete;
	~ServiceLocator() = delete;

public:
	static void initLoggers();
	static Logger getLogger(const String &logger = LOG_DEFAULT);

	static MeshLibrary *getMeshLibrary();

	static void initGLFW();
	static void terminateGLFW();

	// must be called after getMainWindow
	static void initGLEW();
	static GLFWwindow *getMainWindow();

	void glfwErrorCallback(int error, const char *desc);

};

}

#endif // __SERVICE_LOCATOR_H__
