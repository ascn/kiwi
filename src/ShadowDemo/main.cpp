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
#include "Scene/Light.h"

#include "RotatingMesh.h"
#include "MeshObject.h"
//#include "OrbitingObject.h"

K_COMPONENT_S(RotatingBehavior);

void loadTestScene1(Kiwi::Engine &engine) {
	engine.currentScene = std::make_unique<Kiwi::Scene>();

	// Load material shaders
	Kiwi::ShaderProgram *testShader = new Kiwi::ShaderProgram("TestShader");
	testShader->loadShader(Kiwi::ShaderType::VERTEX_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/vert.glsl");
	testShader->loadShader(Kiwi::ShaderType::FRAGMENT_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/frag.glsl");
	testShader->linkProgram();
	Kiwi::Material *testMat = new Kiwi::Material(*testShader);

	// Add geometry
	Kiwi::Mesh *wahooMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/wahoo.obj", "Wahoo");
	wahooMesh->prepareToDraw();

	RotatingMesh *wahooRotatingObject = new RotatingMesh("Wahoo", wahooMesh, testMat, engine.currentScene.get());

	engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(wahooRotatingObject));

	// Add lights
	Kiwi::GameObject *mainLight = new Kiwi::GameObject("Light");
	auto lightComponent = mainLight->AddComponent<Kiwi::Light>();
	lightComponent.lightType = Kiwi::Light::LightType::Directional;
	lightComponent.shadowType = Kiwi::Light::ShadowType::Realtime;
	mainLight->GetComponent<Kiwi::Transform>()->Translate(Vector3(2, 5, 0));
	mainLight->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 0, 0));
	engine.currentScene->lightList.push_back(&lightComponent);
	engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(mainLight));

	// Add main camera
	Kiwi::GameObject *cameraObject = new Kiwi::GameObject("Camera");
	auto cameraComponent = cameraObject->AddComponent<Kiwi::Camera>();
	cameraObject->GetComponent<Kiwi::Transform>()->Translate(Vector3(0, 2, -10));
	cameraObject->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 0, 0));
	engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(cameraObject));
	engine.currentScene->activeCamera = cameraObject;
}

void loadTestScene2(Kiwi::Engine &engine) {
	engine.currentScene = std::make_unique<Kiwi::Scene>();

	// Load material shaders
	Kiwi::ShaderProgram *testShader = new Kiwi::ShaderProgram("TestShader");
	testShader->loadShader(Kiwi::ShaderType::VERTEX_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/vert.glsl");
	testShader->loadShader(Kiwi::ShaderType::FRAGMENT_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/frag.glsl");
	testShader->linkProgram();
	Kiwi::Material *testMat = new Kiwi::Material(*testShader);

	// Load geometry
	Kiwi::Mesh *wahooMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_wahoo.obj", "Wahoo");
	wahooMesh->prepareToDraw();
	Kiwi::Mesh *boxMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_box.obj", "Box");
	boxMesh->prepareToDraw();
	Kiwi::Mesh *coneMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_cone.obj", "Cone");
	coneMesh->prepareToDraw();
	Kiwi::Mesh *floorMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_floor.obj", "Floor");
	floorMesh->prepareToDraw();
	Kiwi::Mesh *sphereMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_sphere.obj", "Sphere");
	sphereMesh->prepareToDraw();

	// Add geometry objects
	auto wahooObject = std::make_unique<MeshObject>("Wahoo", wahooMesh,
													testMat, engine.currentScene.get());
	engine.currentScene->objects.push_back(std::move(wahooObject));
	auto boxObject = std::make_unique<MeshObject>("Box", boxMesh,
													testMat, engine.currentScene.get());
	engine.currentScene->objects.push_back(std::move(boxObject));
	auto coneObject = std::make_unique<MeshObject>("cone", coneMesh,
													testMat, engine.currentScene.get());
	engine.currentScene->objects.push_back(std::move(coneObject));
	auto floorObject = std::make_unique<MeshObject>("floor", floorMesh,
													testMat, engine.currentScene.get());
	engine.currentScene->objects.push_back(std::move(floorObject));
	auto sphereObject = std::make_unique<MeshObject>("sphere", sphereMesh,
													testMat, engine.currentScene.get());
	engine.currentScene->objects.push_back(std::move(sphereObject));

	// Add lights
	Kiwi::GameObject *mainLight = new Kiwi::GameObject("Light");
	auto lightComponent = mainLight->AddComponent<Kiwi::Light>();
	lightComponent.lightType = Kiwi::Light::LightType::Directional;
	lightComponent.shadowType = Kiwi::Light::ShadowType::Realtime;
	mainLight->GetComponent<Kiwi::Transform>()->Translate(Vector3(2, 10, 0));
	mainLight->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 0, 0));
	engine.currentScene->lightList.push_back(&lightComponent);
	engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(mainLight));

	// Add main camera
	Kiwi::GameObject *cameraObject = new Kiwi::GameObject("Camera");
	auto cameraComponent = cameraObject->AddComponent<Kiwi::Camera>();
	cameraObject->GetComponent<Kiwi::Transform>()->Translate(Vector3(0, 4, -10));
	cameraObject->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 4, 0));
	engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(cameraObject));
	engine.currentScene->activeCamera = cameraObject;
}

int main() {
	Kiwi::Engine engine;

	{
		engine.currentScene = std::make_unique<Kiwi::Scene>();

		// Load material shaders
		Kiwi::ShaderProgram *testShader = new Kiwi::ShaderProgram("TestShader");
		testShader->loadShader(Kiwi::ShaderType::VERTEX_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/shadowForward.vert.glsl");
		testShader->loadShader(Kiwi::ShaderType::FRAGMENT_SHADER, "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/lambertShadow.frag.glsl");
		testShader->linkProgram();
		Kiwi::Material *testMat = new Kiwi::Material(*testShader);

		// Load geometry
		Kiwi::Mesh *wahooMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_wahoo.obj", "Wahoo");
		wahooMesh->prepareToDraw();
		Kiwi::Mesh *boxMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_box.obj", "Box");
		boxMesh->prepareToDraw();
		Kiwi::Mesh *coneMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_cone.obj", "Cone");
		coneMesh->prepareToDraw();
		Kiwi::Mesh *floorMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_floor.obj", "Floor");
		floorMesh->prepareToDraw();
		Kiwi::Mesh *sphereMesh = Kiwi::Mesh::LoadFromFile("D:/Users/achan/Projects/kiwi/kiwi/res/test2/test2_sphere.obj", "Sphere");
		sphereMesh->prepareToDraw();

		// Add geometry objects
		auto wahooObject = std::make_unique<MeshObject>("Wahoo", wahooMesh,
														testMat, engine.currentScene.get());
		engine.currentScene->objects.push_back(std::move(wahooObject));
		auto boxObject = std::make_unique<MeshObject>("Box", boxMesh,
													  testMat, engine.currentScene.get());
		engine.currentScene->objects.push_back(std::move(boxObject));
		auto coneObject = std::make_unique<MeshObject>("cone", coneMesh,
													   testMat, engine.currentScene.get());
		engine.currentScene->objects.push_back(std::move(coneObject));
		auto floorObject = std::make_unique<MeshObject>("floor", floorMesh,
														testMat, engine.currentScene.get());
		engine.currentScene->objects.push_back(std::move(floorObject));
		auto sphereObject = std::make_unique<MeshObject>("sphere", sphereMesh,
														 testMat, engine.currentScene.get());
		engine.currentScene->objects.push_back(std::move(sphereObject));

		// Add lights
		Kiwi::GameObject *mainLight = new Kiwi::GameObject("Light");
		auto lightComponent = mainLight->AddComponent<Kiwi::Light>();
		lightComponent.lightType = Kiwi::Light::LightType::Directional;
		lightComponent.shadowType = Kiwi::Light::ShadowType::Realtime;
		mainLight->GetComponent<Kiwi::Transform>()->Translate(Vector3(2, 8, 0));
		mainLight->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 0, 0));
		engine.currentScene->lightList.push_back(&lightComponent);
		engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(mainLight));

		// Add main camera
		Kiwi::GameObject *cameraObject = new Kiwi::GameObject("Camera");
		auto cameraComponent = cameraObject->AddComponent<Kiwi::Camera>();
		cameraObject->GetComponent<Kiwi::Transform>()->Translate(Vector3(0, 4, -20));
		cameraObject->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 4, 0));
		engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(cameraObject));
		engine.currentScene->activeCamera = cameraObject;
	}
	
	// Run
	engine.run();
	return 0;
}