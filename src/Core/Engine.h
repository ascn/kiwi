#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "Scene/Scene.h"

namespace Kiwi {

class Engine {
public:
	Engine();
	~Engine();

	Engine(const Engine &) = delete;
	Engine &operator=(const Engine) = delete;

	void setTestScene();

	void run();

	std::unique_ptr<Scene> currentScene;
};

}

#endif // __ENGINE_H__