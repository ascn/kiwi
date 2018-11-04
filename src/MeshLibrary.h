#ifndef __MESH_LIBRARY_H__
#define __MESH_LIBRARY_H__

#include "util.h"

namespace Kiwi {

class Mesh;

class MeshLibrary {
public:
	Mesh *getMesh(String name);
	void addMesh(String name, Mesh *mesh);

private:
	std::unordered_map<String, Mesh *> meshLib;
};

}

#endif // __MESH_LIBRARY_H__