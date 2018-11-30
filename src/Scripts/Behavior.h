#ifndef __BEHAVIOR_H__
#define __BEHAVIOR_H__

#include "Core/Component.h"

namespace Kiwi {

class Behavior;
using BehaviorList = std::vector<BehaviorList *>;

class Behavior : public Component {
public:
	Behavior();
	virtual ~Behavior() {}
	virtual void update() = 0;

	virtual Component *clone() = 0;

	static BehaviorList &getBehaviorList() {
		static BehaviorList behaviorList;
		return behaviorList;
	}

	bool enabled;
}

}

#endif // __BEHAVIOR_H__