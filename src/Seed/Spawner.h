#ifndef __SPAWNER_H__
#define __SPAWNER_H__

#include "Core/Object.h"
#include <any>

namespace Kiwi::Seed {

class Emitter;

class Spawner : public Object {
public:
	Spawner(Emitter *e);
	virtual ~Spawner() {}

	virtual const std::unordered_map<String, std::any> &GetSpawnedProperties() = 0;

protected:
	Emitter *emitter;
	std::unordered_map<String, std::any> nextSpawnedProperties;
};

}

#endif // __SPAWNER_H__