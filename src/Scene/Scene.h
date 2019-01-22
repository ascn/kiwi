#ifndef __SCENE_H__
#define __SCENE_H__

#include "Core/Object.h"
#include "Core/GameObject.h"
#include "Rendering/Renderer.h"
#include "Scene/Camera.h"
#include "Core/Engine.h"

namespace Kiwi {

class Scene : public Object {
public:
	Scene();
	~Scene();

	void update();
	void render();

	GameObject *activeCamera;

	friend class Renderer;
	friend class Behavior;
	friend class Engine;

	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<Renderer *> renderList;
	std::vector<Behavior *> updateList;
	MeshLibrary meshLib;
};

}

#endif // __SCENE_H__