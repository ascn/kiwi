#include "Rendering/Mesh.h"
#include "Core/ServiceLocator.h"
#include "Logger.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Kiwi {

Mesh::Mesh() :
	vertices(),
	indices(),
	name("") {}

Mesh::Mesh(String name) :
	vertices(),
	indices(),
	name(name) {}

Mesh *Mesh::LoadFromFile(const String &filename) {
	return Mesh::LoadFromFile(filename, filename);
}

Mesh *Mesh::LoadFromFile(const String &filename, const String &name) {
	Assimp::Importer importer;
	bool isError = false;
	bool isAllocated = false;
	Mesh *ret = nullptr;
	const aiScene *scene = importer.ReadFile(filename,
			aiProcessPreset_TargetRealtime_MaxQuality
	);

	if (!scene) {
		ServiceLocator::getLogger(LOG_RENDERING).Log(importer.GetErrorString());
		return ret;
	}

	auto m = std::make_unique<Mesh>(name);
	for (int i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh *currMesh = scene->mMeshes[i];
		if (!(currMesh->HasPositions()) || !(currMesh->HasFaces())) { continue; }

		int idxOffset = m->vertices.size();
		// Load vertex positions
		for (int j = 0; j < currMesh->mNumVertices; ++j) {
			aiVector3D pos = currMesh->mVertices[j];
			aiVector3D nor = currMesh->HasNormals() ? currMesh->mNormals[j] : aiVector3D(0, 0, 1);
			m->vertices.push_back({ { pos.x, pos.y, pos.z },
									{ nor.x, nor.y, nor.z },
									{ 0, 0 } });
		}

		// Load vertex uvs
		if (currMesh->HasTextureCoords(0)) {
			for (int j = 0; j < currMesh->mNumVertices; ++j) {
				// TODO: figure out how to get tex coords
			}
		}

		for (int j = 0; j < currMesh->mNumFaces; ++j) {
			aiFace face = currMesh->mFaces[j];
			if (face.mNumIndices != 3) {
				ServiceLocator::getLogger(LOG_RENDERING).Log("Face is not a triangle");
				return ret;
			}
			m->indices.push_back(idxOffset + face.mIndices[0]);
			m->indices.push_back(idxOffset + face.mIndices[1]);
			m->indices.push_back(idxOffset + face.mIndices[2]);
		}
	}
	ret = m.get();
	ServiceLocator::getMeshLibrary()->addMesh(name, std::move(m));
	return ret;
}

void Mesh::prepareToDraw() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tex));

	glBindVertexArray(0);
}

K_COMPONENT_S(MeshFilter)
MeshFilter::MeshFilter() :
	mesh(nullptr) {}

MeshFilter::MeshFilter(Mesh *m) :
	mesh(m) {}

Mesh *MeshLibrary::getMesh(String name) {
	Mesh *ret = nullptr;
	try {
		ret = meshLib.at(name).get();
	} catch (std::exception &) {
		ServiceLocator::getLogger().Log("Failed to retrieve mesh: " + name);
	}
	return ret;
}

void MeshLibrary::addMesh(String name, std::unique_ptr<Mesh> mesh) {
	meshLib.emplace(name, std::move(mesh));
}

}