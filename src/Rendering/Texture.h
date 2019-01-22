#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>
#include <SOIL.h>
#include "Core/Object.h"

namespace Kiwi {

class Texture : public Object {
public:
	Texture();
	~Texture();

	static Texture *LoadFromFile(const String &filename);

	void prepareToUse();

	struct uchar4 {
		unsigned char data[4];
		unsigned char r() { return data[0]; }
		unsigned char g() { return data[1]; }
		unsigned char b() { return data[2]; }
		unsigned char a() { return data[3]; }
	};

private:
	int width;
	int height;
	std::vector<uchar4> data;
	GLuint textureID;
};

class TextureLibrary {
public:
	/**
	 * Returns a non-owning pointer to a mesh with the given
	 * name \p name. If no such mesh exists, nullptr is returned.
	 *
	 * @param  name the name of the mesh to retrieve
	 * @return      pointer to the meshif one exists, nullptr otherwise
	 */
	Texture *getTexture(String name);

	/**
	 * Adds a mesh to the library. The library will take
	 * ownership of the mesh, and the unique_ptr containing
	 * the mesh will now be nullptr after this function.
	 *
	 * @param name name of the mesh
	 * @param mesh unique_ptr to the mesh
	 */
	void addTexture(String name, std::unique_ptr<Texture> texture);

private:
	std::unordered_map<String, std::unique_ptr<Texture>> texLib;
};

}

#endif // __TEXTURE_H__