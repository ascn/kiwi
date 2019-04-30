#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Core/Engine.h"
#include "Core/ServiceLocator.h"
#include "Scene/Transform.h"
#include "ShadowDemo/MeshObject.h"

int main() {
	Kiwi::Engine engine;
	try {
		engine.currentScene = std::make_unique<Kiwi::Scene>();

		// Load passthrough and raymarch shader
		Kiwi::ShaderProgram *raymarchShader = new Kiwi::ShaderProgram("RaymarchShader");
		raymarchShader->loadShader(Kiwi::ShaderType::VERTEX_SHADER, 
			"D:/Users/achan/Projects/kiwi/kiwi/src/botw-proc/passthrough.vert.glsl");
		raymarchShader->loadShader(Kiwi::ShaderType::FRAGMENT_SHADER, 
			"D:/Users/achan/Projects/kiwi/kiwi/src/botw-proc/raymarch.frag.glsl");
		raymarchShader->linkProgram();
		Kiwi::Material *quadMaterial = new Kiwi::Material(*raymarchShader);

		// Create screen space quad
		auto quadMeshUP = std::make_unique<Kiwi::Mesh>("ScreenQuad");
		Kiwi::Mesh *quadMesh = quadMeshUP.get();
		Kiwi::ServiceLocator::getMeshLibrary()->addMesh("ScreenQuad", std::move(quadMeshUP));

		quadMesh->vertices.push_back({ { -1, -1, 0 },
									   { 0, 0, 1 },
									   { 0, 0 } });
		quadMesh->vertices.push_back({ { 1, -1, 0 },
									   { 0, 0, 1 },
									   { 1, 0 } });
		quadMesh->vertices.push_back({ { 1, 1, 0 },
									   { 0, 0, 1 },
									   { 1, 1 } });
		quadMesh->vertices.push_back({ { -1, 1, 0 },
									   { 0, 0, 1 },
									   { 0, 1 } });

		quadMesh->indices = Vector<unsigned int>({ 0, 1, 2, 0, 2, 3 });
		quadMesh->prepareToDraw();

		auto quadObject = std::make_unique<MeshObject>("Quad", quadMesh,
													   quadMaterial, engine.currentScene.get());
		engine.currentScene->objects.push_back(std::move(quadObject));
		
		// Create camera
		Kiwi::GameObject *cameraObject = new Kiwi::GameObject("Camera");
		auto cameraComponent = cameraObject->AddComponent<Kiwi::Camera>();
		engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(cameraObject));
		engine.currentScene->activeCamera = cameraObject;
	} catch (std::exception &e) {
		return -1;
	}

	engine.run();
	return 0;
}