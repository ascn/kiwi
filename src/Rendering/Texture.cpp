#include "Rendering/Texture.h"
#include "Core/ServiceLocator.h"

namespace Kiwi {

Texture::Texture() :
	data(),
	textureID(-1)
{}

Texture::~Texture() {
	if (textureID != -1) {
		glDeleteTextures(1, &textureID);
	}
}

Texture *Texture::CreateEmptyTexture(int width, int height) {
	auto t = new Texture();
	t->width = width;
	t->height = height;
	glGenTextures(1, &t->textureID);
	glBindTexture(GL_TEXTURE_2D, t->textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return t;
}

Texture *Texture::LoadFromFile(const String &filename) {
	/*
	int channels;
	auto t = std::make_unique<Texture>();
	unsigned char *tex = SOIL_load_image(filename.c_str(),
										 &t->width, &t->height, &channels,
										 SOIL_LOAD_AUTO);
	for (int i = 0; i < t->width * t->height * channels; i += channels) {
		unsigned char a = channels == 4 ? tex[i + 3] : 255;
		t->data.push_back({ tex[i + 0],
							tex[i + 1],
							tex[i + 2],
							a });
	}

	SOIL_free_image_data(tex);

	glGenTextures(1, &t->textureID);
	Texture *ret = t.get();
	ServiceLocator::getTextureLibrary()->addTexture(filename, std::move(t));
	return ret;
	*/
	return nullptr;
}

void Texture::prepareToUse() {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				 width, height, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

Texture *TextureLibrary::getTexture(String name) {
	Texture *ret = nullptr;
	try {
		ret = texLib.at(name).get();
	} catch (std::exception &) {
		ServiceLocator::getLogger().Log("Failed to retrieve mesh: " + name);
	}
	return ret;
}

void TextureLibrary::addTexture(String name, std::unique_ptr<Texture> texture) {
	texLib.emplace(name, std::move(texture));
}

}