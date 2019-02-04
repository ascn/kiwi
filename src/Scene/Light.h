#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Core/Component.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Framebuffer.h"

namespace Kiwi {

class Light : public Component {
K_COMPONENT_H(Light)
public:
	Light();
	virtual ~Light() override {}

	enum class LightType : char {
		Directional,
		Point
	} lightType;

	enum class ShadowType : char {
		Realtime,
		None
	} shadowType;

	int width, height;

	void prepareToRenderShadowMap();
	Matrix4 getLightSpaceTransform();

	static ShaderProgram *initializeShadowShader();
	static ShaderProgram *shadowShader;

	virtual Component *clone() override {
		return nullptr;
	}

private:
	friend class Scene;
	Framebuffer depthFBO;
};

}

#endif // __LIGHT_H__