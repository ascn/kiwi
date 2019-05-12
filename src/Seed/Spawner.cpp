#include "Seed/Spawner.h"
#include "Seed/Emitter.h"

namespace Kiwi::Seed {

Spawner::Spawner(Emitter *e) :
	emitter(e),
	nextSpawnedProperties()
{
	const auto &propertyNames = emitter->manager.getPropertyNames();
	for (const auto &name : propertyNames) {
		nextSpawnedProperties.emplace(name, std::any());
	}
}

const std::unordered_map<String, std::any> &Spawner::GetSpawnedProperties() {
	ComputeSpawnedProperties();
	return nextSpawnedProperties;
}

}