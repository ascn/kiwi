#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Core/GameObject.h"
#include "Core/ServiceLocator.h"
#include "Logger.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Mesh.h"
#include "Rendering/Renderer.h"
#include "Rendering/MeshRenderer.h"

int mainInit() {
	try {
		Kiwi::ServiceLocator::initLoggers();
	} catch (std::exception &e) {
		std::cout << "error init loggers: " << e.what() << std::endl;
		return 0;
	}
	try {
		Kiwi::ServiceLocator::initGLFW();
		Kiwi::ServiceLocator::getMainWindow();
		Kiwi::ServiceLocator::initGLEW();
	} catch (std::exception &e) {
		return 0;
	}

	return 1;
}

int mainTerminate() {
	Kiwi::ServiceLocator::terminateGLFW();
	return 1;
}

void setUpTest() {
	try {
		Kiwi::ShaderProgram *testShader = new Kiwi::ShaderProgram("TestShader");
		testShader->loadShader(Kiwi::ShaderType::VERTEX_SHADER, "../../src/Shaders/vert.glsl");
		testShader->loadShader(Kiwi::ShaderType::FRAGMENT_SHADER, "../../src/Shaders/frag.glsl");
		testShader->linkProgram();
		Kiwi::Material *testMat = new Kiwi::Material(*testShader);

		Kiwi::Mesh *mesh = new Kiwi::Mesh("tri");
		Kiwi::Mesh::Vertex v1 = { {-1.f, -1.f, 0.f },
								  { 1.f, 0.f, 0.f },
								  { 0.f, 0.f} };
		Kiwi::Mesh::Vertex v2 = { { 1.f, -1.f, 0.f },
								  { 1.f, 0.f, 0.f },
								  { 0.f, 0.f} };
		Kiwi::Mesh::Vertex v3 = { { 0.f, 1.f, 0.f },
								  { 1.f, 0.f, 0.f },
								  { 0.f, 0.f} };
		mesh->vertices = { v1, v2, v3 };
		mesh->indices = { 0, 1, 2 };
		mesh->prepareToDraw();

		Kiwi::GameObject *obj = new Kiwi::GameObject("Test Object");
		obj->AddComponent<Kiwi::MeshFilter>()->mesh = mesh;
		obj->AddComponent<Kiwi::MeshRenderer>()->material = testMat;
	} catch (std::exception &) {
		exit (1);
	}
}

void renderTest() {
	for (Kiwi::Renderer *r : Kiwi::Renderer::getRendererList()) {
		r->render();
	}
}

int main() {
	if (!mainInit()) {
		return 0;
	}

	GLFWwindow *mainWindow = Kiwi::ServiceLocator::getMainWindow();

	setUpTest();
	Kiwi::ShaderProgram *shader =
		Kiwi::ShaderProgram::getShaderLibrary().at("TestShader");

	glDisable(GL_CULL_FACE);

	do {
		glClear(GL_COLOR_BUFFER_BIT);

		renderTest();

		glfwSwapBuffers(mainWindow);

		glfwPollEvents();
	} while (glfwWindowShouldClose(mainWindow) == 0);

	mainTerminate();
	return 0;
}