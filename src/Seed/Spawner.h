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

	const std::unordered_map<String, std::any> &GetSpawnedProperties();

protected:
	virtual void ComputeSpawnedProperties() = 0;

	Emitter *emitter;
	std::unordered_map<String, std::any> nextSpawnedProperties;
};

}

#endif // __SPAWNER_H__