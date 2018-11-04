#include "Rendering/MeshLibrary.h"
#include "Rendering/Mesh.h"
#include "Core/ServiceLocator.h"

namespace Kiwi {

Mesh *MeshLibrary::getMesh(String name) {
	Mesh *ret = nullptr;
	try {
		ret = meshLib.at(name);
	} catch (std::exception &) {
		ServiceLocator::getLogger().Log("Failed to retrieve mesh: " + name);
	}
	return ret;
}

void MeshLibrary::addMesh(String name, Mesh *mesh) {
	meshLib.emplace(name, mesh);
}

}