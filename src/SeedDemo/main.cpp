#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Core/Engine.h"
#include "Seed/SeedSystem.h"
#include "Seed/Emitter.h"
#include "Seed/TickLifetimeModule.h"
#include "Seed/ResetForceModule.h"
#include "Seed/GravityModule.h"
#include "Seed/SeedPointRenderer.h"
#include "Seed/FountainSpawner.h"
#include "Scene/Transform.h"

#include "GlobeSpawner.h"
#include "GlobeModule.h"

#define __FOUNTAIN 1
#define __GLOBE 0

int main() {
	Kiwi::Engine engine;
	try {
		engine.currentScene = std::make_unique<Kiwi::Scene>();
		
		// Load point render shader
		Kiwi::ShaderProgram *pointShader = new Kiwi::ShaderProgram("SeedPointShader");
		pointShader->loadAndLink("D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/seedpoint.vert.glsl",
								 "", "", "",
								 "D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/seedpoint.frag.glsl");

		Kiwi::Material *fountainMaterial = new Kiwi::Material(*pointShader);

		// Create fountain object
		Kiwi::GameObject *fountainObject = new Kiwi::GameObject("Fountain");
		auto &seedSysComp = fountainObject->AddComponent<Kiwi::Seed::SeedSystem>(*(engine.currentScene));
		auto emitter_up = std::make_unique<Kiwi::Seed::Emitter>(&seedSysComp, 1000, [](float elapsedTime) {
			return (glm::cos(0.4f * elapsedTime) + 1) * 100.f;
		});
		Kiwi::Seed::Emitter &emitter = *emitter_up;
		seedSysComp.emitters.push_back(std::move(emitter_up));
		auto &tickModule = emitter.AddModule<Kiwi::Seed::TickLifetimeModule>();
		emitter.AddModule<Kiwi::Seed::ResetForceModule>();
		emitter.AddModule<Kiwi::Seed::GravityModule>().setEnabled(true);
		auto &globeModule = emitter.AddModule<GlobeModule>();
		globeModule.setEnabled(false);
		emitter.AddModule<Kiwi::Seed::PhysicsModule>();
		emitter.AddRenderer<Kiwi::Seed::SeedPointRenderer>(*(engine.currentScene)).material = fountainMaterial;
		
		#if __FOUNTAIN
		auto &fountainSpawner = emitter.SetSpawner<Kiwi::Seed::FountainSpawner>();
		fountainSpawner.lifetime = 3;
		fountainSpawner.angle = glm::radians(180.f);
		fountainSpawner.axis = Vector3(-1, 0, 0);
		#elif __GLOBE
		float radius = 1;
		auto &globeSpawner = emitter.SetSpawner<GlobeSpawner>();
		globeSpawner.radius = radius;
		globeModule.center = fountainObject->GetComponent<Kiwi::Transform>()->GetPosition();
		globeModule.radius = radius;
		globeModule.setEnabled(true);
		tickModule.setEnabled(false);
		#endif
		engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(fountainObject));

		// Create camera
		Kiwi::GameObject *cameraObject = new Kiwi::GameObject("Camera");
		auto cameraComponent = cameraObject->AddComponent<Kiwi::Camera>();
		cameraObject->GetComponent<Kiwi::Transform>()->Translate(Vector3(0, 4, -10));
		cameraObject->GetComponent<Kiwi::Transform>()->LookAt(Vector3(0, 0, 0));
		engine.currentScene->objects.push_back(std::unique_ptr<Kiwi::GameObject>(cameraObject));
		engine.currentScene->activeCamera = cameraObject;

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	engine.run();
	return 0;
}