#include "Scene/Scene.h"
#include "Scene/Transform.h"
#include "Scripts/Behavior.h"

namespace Kiwi {

Scene::Scene() :
	objects(),
	renderList(),
	updateList(),
	meshLib(),
	activeCamera(nullptr)
{
}

Scene::~Scene() {}

void Scene::update() {
	for (const auto &b : updateList) {
		b->update();
	}
}

void Scene::render() {
	// TODO: Traverse the scene graph to update
	// transforms of each game object

	// Render shadow maps
	for (const auto &light : lightList) {
		if (light->shadowType != Light::ShadowType::Realtime) { continue; }
		light->prepareToRenderShadowMap();
		for (const auto &r : renderList) {
			if (!r->castShadows) { continue; }
			auto transform = r->gameObject->GetComponent<Transform>()->GetViewMatrix();
			Light::shadowShader->setUniform("u_model", transform);
			r->render(false);
		}
	}

	auto cameraComponent = activeCamera->GetComponent<Camera>();

	cameraComponent->computeViewMatrix();
	cameraComponent->computeProjectionMatrix();
	
	// Iterate over render components and render
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, ServiceLocator::width, ServiceLocator::height);

	Matrix4 lightView;
	Vector3 lightPos;
	Light *shadowCastingLight = nullptr;
	if (lightList.size() > 0) {
		shadowCastingLight = lightList[0];
		lightView = shadowCastingLight->getLightSpaceTransform();
		lightPos = shadowCastingLight->gameObject->GetComponent<Transform>()->GetPosition();
	}

	for (const auto &r : renderList) {
		auto transform = r->gameObject->GetComponent<Transform>()->GetViewMatrix();
		auto invTranspose = glm::inverse(glm::transpose(transform));
		r->material->shader.setUniform("u_model", transform);
		r->material->shader.setUniform("u_modelInvTr", invTranspose);
		r->material->shader.setUniform("u_view", cameraComponent->viewMatrix());
		r->material->shader.setUniform("u_proj", cameraComponent->projectionMatrix());
		r->material->shader.setUniform("u_resolution", Vector2(1280, 720));

		if (shadowCastingLight) {
			r->material->shader.setUniform("u_lightView", lightView);
			r->material->shader.setUniform("u_lightPos", lightPos);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, shadowCastingLight->depthFBO.depth);
			r->material->shader.setUniform("u_shadowMap", 0);
		}

		r->render();
	}
}

}