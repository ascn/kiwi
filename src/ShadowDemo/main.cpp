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
#include "Scene/Camera.h"
#include "Scene/Transform.h"
#include "Core/Engine.h"

#include "RotatingMesh.h"

K_COMPONENT_S(RotatingBehavior);

int main() {
	Kiwi::Engine engine;

	engine.currentScene = std::make_unique<Kiwi::Scene>();
	Kiwi::ShaderProgram *testShader = new Kiwi::ShaderProgram("TestShader");
	testShader->loadShader(Kiwi::ShaderType::VERTEX_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/vert.glsl");
	testShader->loadShader(Kiwi::ShaderType::FRAGMENT_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/frag.glsl");
	testShader->linkProgram();
	Kiwi::Material *testMat = new Kiwi::Material(*testShader);

	Kiwi::Mesh *wahooMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/wahoo.obj", "Wahoo");
	wahooMesh->prepareToDraw();

	RotatingMesh *wahooRotatingObject = new RotatingMesh("Wahoo", wahooMesh, testMat, engine.currentScene.get());

	engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(wahooRotatingObject));

	Kiwi::GameObject *cameraObject = new Kiwi::GameObject("Camera");
	auto cameraComponent = cameraObject->AddComponent<Kiwi::Camera>();
	cameraObject->GetComponent<Kiwi::Transform>()->Translate(Vector3(0, 2, -10));
	cameraObject->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 0, 0));
	engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(cameraObject));
	//currentScene->setActiveCamera(cameraComponent);
	engine.currentScene->activeCamera = cameraObject;

	engine.run();
	return 0;
}