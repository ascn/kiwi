#ifndef __BEHAVIOR_H__
#define __BEHAVIOR_H__

#include "Core/Component.h"
#include "Scene/Scene.h"

namespace Kiwi {

class Behavior;
using BehaviorList = std::vector<Behavior *>;

class Behavior : public Component {
public:
	Behavior() : enabled(true) {}
	Behavior(Scene &s) : enabled(true), s(&s)
	{
		s.updateList.push_back(this);
	}

	virtual ~Behavior() {}
	virtual void update() = 0;

	virtual Component *clone() = 0;

	static BehaviorList &getBehaviorList() {
		static BehaviorList behaviorList;
		return behaviorList;
	}

	bool enabled;
	Scene *s;
};

}

#endif // __BEHAVIOR_H__