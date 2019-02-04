#include <GL/glew.h>

#include "Core/GameObject.h"
#include "Scene/Light.h"
#include "Scene/Transform.h"

namespace Kiwi {

K_COMPONENT_S(Light)

Light::Light() :
	lightType(LightType::Directional),
	shadowType(ShadowType::None),
	width(1024),
	height(1024),
	depthFBO("Shadow Map", width, height)
{
	depthFBO.attachNewDepthBuffer();
	if (shadowShader == nullptr) {
		shadowShader = Light::initializeShadowShader();
	}
}

void Light::prepareToRenderShadowMap() {
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	shadowShader->useProgram();
	shadowShader->setUniform("u_lightView", getLightSpaceTransform());
}

Matrix4 Light::getLightSpaceTransform() {
	Matrix4 lightProj = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 10.f);
	auto transform = Component::gameObject->GetComponent<Transform>();
	Matrix4 lightView = transform->GetViewMatrix();
	return lightProj * lightView;
}

ShaderProgram *Light::initializeShadowShader() {
	ShaderProgram *ret = new ShaderProgram("Shadow Shader");
	ret->loadAndLink("D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/shadowdepth.vert.glsl",
					"", "", "",
					"D:/Users/achan/Projects/kiwi/kiwi/src/Shaders/shadowdepth.frag.glsl");
	return ret;
}

ShaderProgram *Light::shadowShader = nullptr;

}