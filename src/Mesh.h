#ifndef __MESH_H__
#define __MESH_H__

#include <GL/glew.h>

#include "util.h"
#include "Object.h"
#include "Component.h"

namespace Kiwi {

/**
 * Class representing a mesh allowing access and modification
 * through scripts.
 */
class Mesh : public Object {
public:
	Mesh();
	Mesh(String name);

	struct Vertex {
		Vector3 position;
		Vector3 normal;
		Vector2 tex;
	};

	/** @name LoadFromFile
	 * Loads a mesh from a file. Returns a pointer to the
	 * loaded mesh. If loading fails, nullptr is returned.
	 */
	/**@{*/
	/**
	 * Loads a mesh from \p filename. The resulting mesh
	 * will take the name of \p filename.
	 */
	static Mesh *LoadFromFile(const String &filename);
	/**
	 * Loads a mesh from \p filename. The resulting mesh
	 * will take the name designated by \p name.
	 */
	static Mesh *LoadFromFile(const String &filename, const String &name);
	/**@}*/

	/**
	 * Create a vertex array object and VBOs for position,
	 * normal, and texture coordinates. Copies data from
	 * #vertices, #normals, and #uv. Index buffer is copied
	 * from #triangles.
	 */
	void prepareToDraw();

	Vector<Vertex> vertices;

	/** Face indices */
	Vector<unsigned int> indices;

	/** Name of the mesh */
	String name;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};

class MeshFilter : public Component {
public:
K_COMPONENT_H(MeshFilter)
	MeshFilter();
	MeshFilter(Mesh *m);

	Component *clone() override {
		return new MeshFilter(mesh);
	}

	Mesh *mesh;
};

}

#endif // __MESH_H__