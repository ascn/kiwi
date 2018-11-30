#ifndef __MESH_H__
#define __MESH_H__

#include <GL/glew.h>

#include "util.h"
#include "Core/Object.h"
#include "Core/Component.h"

namespace Kiwi {

/**
 * Class representing a mesh allowing access and modification
 * through scripts.
 */
class Mesh : public Object {
friend class MeshRenderer;
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

private:
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

class MeshLibrary {
public:
	/**
	 * Returns a non-owning pointer to a mesh with the given
	 * name \p name. If no such mesh exists, nullptr is returned.
	 *
	 * @param  name the name of the mesh to retrieve
	 * @return      pointer to the meshif one exists, nullptr otherwise
	 */
	Mesh *getMesh(String name);

	/**
	 * Adds a mesh to the library. The library will take
	 * ownership of the mesh, and the unique_ptr containing
	 * the mesh will now be nullptr after this function.
	 * 
	 * @param name name of the mesh
	 * @param mesh unique_ptr to the mesh
	 */
	void addMesh(String name, std::unique_ptr<Mesh> mesh);

private:
	std::unordered_map<String, std::unique_ptr<Mesh>> meshLib;
};

}

#endif // __MESH_H__