#include <sstream>

#include "Core/ServiceLocator.h"
#include "Logger.h"

namespace Kiwi {

void ServiceLocator::initLoggers() {
	Logger::Initialize();
}

Logger ServiceLocator::getLogger(const String &logger) {
	if (!Logger::initialized) { return Logger(); }
	std::shared_ptr<spdlog::logger> log;
	try {
		log = Logger::logs.at(logger);
	} catch (std::exception &) {
		Logger::logs.emplace(logger, spdlog::basic_logger_mt(logger, "log/" + logger + ".txt", true));
		log = Logger::logs.at(logger);
		log->set_level(Logger::level);
	}
	return Logger(log);
}

MeshLibrary *ServiceLocator::getMeshLibrary() {
	static MeshLibrary meshLib;
	return &meshLib;
}

TextureLibrary *ServiceLocator::getTextureLibrary() {
	static TextureLibrary texLib;
	return &texLib;
}

void ServiceLocator::initGLFW() {
	if (!glfwInit()) {
		ServiceLocator::getLogger(LOG_RENDERING).Log("Failed to initialize GLFW", LOG_LEVEL::err);
		throw std::runtime_error("Failed to initialize GLFW");
	} else {
		ServiceLocator::getLogger().Log("Initialized GLFW");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

int ServiceLocator::width = 1280;
int ServiceLocator::height = 720;

GLFWwindow *ServiceLocator::getMainWindow() {
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//static GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Kiwi", monitor, NULL);
	static GLFWwindow *window = glfwCreateWindow(width, height, "Kiwi", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	return window;
}

void ServiceLocator::initGLEW() {
	glewExperimental = true;

	if (glewInit()) {
		ServiceLocator::getLogger(LOG_RENDERING).Log("Failed to initialize GLEW", LOG_LEVEL::err);
		throw std::runtime_error("Failed to initialize GLEW");
	} else {
		std::stringstream sstr;
		sstr << glewGetString(GLEW_VERSION);
		ServiceLocator::getLogger().Log("Initialized GLEW with version " + sstr.str());
		sstr.clear();
		int major, minor;
		glfwGetVersion(&major, &minor, NULL);
		sstr << major << "." << minor;
		ServiceLocator::getLogger().Log("Using OpenGL version " + sstr.str());
	}
}

void ServiceLocator::terminateGLFW() {
	glfwTerminate();
}

void ServiceLocator::glfwErrorCallback(int error, const char *desc) {
	ServiceLocator::getLogger(LOG_RENDERING).Log(desc, LOG_LEVEL::err);
}

}