#include "Scene/Scene.h"
#include "Scene/Transform.h"
#include "Scripts/Behavior.h"

namespace Kiwi {

Scene::Scene() :
	objects(),
	renderList(),
	updateList(),
	meshLib(),
	activeCamera(nullptr) {}

Scene::~Scene() {}

void Scene::update() {
	for (const auto &b : updateList) {
		b->update();
	}
}

void Scene::render() {
	// TODO: Traverse the scene graph to update
	// transforms of each game object

	auto cameraComponent = activeCamera->GetComponent<Camera>();

	cameraComponent->computeViewMatrix();
	cameraComponent->computeProjectionMatrix();
	
	// Iterate over render components and render
	for (const auto &r : renderList) {
		auto transform = r->gameObject->GetComponent<Transform>()->GetViewMatrix();
		auto invTranspose = glm::inverse(glm::transpose(transform));
		r->material->shader.setUniform("u_model", transform);
		r->material->shader.setUniform("u_modelInvTr", invTranspose);
		r->material->shader.setUniform("u_view", cameraComponent->viewMatrix());
		r->material->shader.setUniform("u_proj", cameraComponent->projectionMatrix());

		r->render();
	}
}

}