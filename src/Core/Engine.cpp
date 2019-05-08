#include <chrono>

#include "Core/Engine.h"
#include "ServiceLocator.h"
#include "Scene/Scene.h"
#include "Core/GameObject.h"
#include "Scene/Transform.h"
#include "Rendering/MeshRenderer.h"

namespace Kiwi {

Engine::Engine() {
	ServiceLocator::initLoggers();
	ServiceLocator::initGLFW();
	ServiceLocator::getMainWindow();
	ServiceLocator::initGLEW();
}

Engine::~Engine() {
	ServiceLocator::terminateGLFW();
}

void Engine::setTestScene() {
	currentScene = std::make_unique<Scene>();
	Kiwi::ShaderProgram *testShader = new Kiwi::ShaderProgram("TestShader");
	testShader->loadShader(Kiwi::ShaderType::VERTEX_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/vert.glsl");
	testShader->loadShader(Kiwi::ShaderType::FRAGMENT_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/frag.glsl");
	testShader->linkProgram();
	Kiwi::Material *testMat = new Kiwi::Material(*testShader);

	Kiwi::Mesh *wahooMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/wahoo.obj", "Wahoo");
	wahooMesh->prepareToDraw();

	Kiwi::GameObject *wahooObject = new Kiwi::GameObject("Wahoo");
	wahooObject->AddComponent<Kiwi::MeshFilter>(wahooMesh);
	wahooObject->AddComponent<Kiwi::MeshRenderer>(*currentScene).material = testMat;

	currentScene->objects.push_back(std::unique_ptr<GameObject>(wahooObject));

	Kiwi::GameObject *cameraObject = new Kiwi::GameObject("Camera");
	auto cameraComponent = cameraObject->AddComponent<Kiwi::Camera>();
	cameraObject->GetComponent<Kiwi::Transform>()->Translate(Vector3(0, 2, -10));
	cameraObject->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 0, 0));
	currentScene->objects.push_back(std::unique_ptr<GameObject>(cameraObject));
	//currentScene->setActiveCamera(cameraComponent);
	currentScene->activeCamera = cameraObject;
}

constexpr int TICK_RATE = 32;
constexpr double MS_PER_TICK = 1000.0 / static_cast<double>(TICK_RATE);

void Engine::run() {
	GLFWwindow *mainWindow = ServiceLocator::getMainWindow();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	auto previous = std::chrono::high_resolution_clock::now();
	double lag = 0.0;

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto current = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = current - previous;
		previous = current;
		lag += elapsed.count();

		while (lag >= MS_PER_TICK) {
			// Update
			currentScene->update(MS_PER_TICK / 1000.f);
			lag -= MS_PER_TICK;
		}

		// Render
		currentScene->render();

		glfwSwapBuffers(mainWindow);

		glfwPollEvents();

	} while (glfwWindowShouldClose(mainWindow) == 0);
}

}