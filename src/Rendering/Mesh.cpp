#include "Rendering/Mesh.h"
#include "Core/ServiceLocator.h"
#include "Logger.h"
#include "Rendering/MeshLibrary.h"

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

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh *currMesh = scene->mMeshes[i];
		if (!(currMesh->HasPositions()) || !(currMesh->HasFaces())) { continue; }
		Mesh *m = new Mesh(name);
		isAllocated = true;
		
		// Load vertex positions
		for (int j = 0; j < currMesh->mNumVertices; ++j) {
			aiVector3D pos = currMesh->mVertices[j];
			//m->vertices.push_back(Vector3(pos.x, pos.y, pos.z));
		}

		// Load vertex normals
		if (currMesh->HasNormals()) {
			for (int j = 0; j < currMesh->mNumVertices; ++j) {
				aiVector3D nor = currMesh->mNormals[j];
				//m->normals.push_back(Vector3(nor.x, nor.y, nor.z));
			}
		}

		// Load vertex uvs
		if (currMesh->HasTextureCoords(0)) {
			for (int j = 0; j < currMesh->mNumVertices; ++j) {
				aiVector3D *uv = currMesh->mTextureCoords[j];
				// TODO: figure out how to get tex coords
			}
		}

		for (int j = 0; j < currMesh->mNumFaces; ++j) {
			aiFace face = currMesh->mFaces[j];
			if (face.mNumIndices != 3) {
				ServiceLocator::getLogger(LOG_RENDERING).Log("Face is not a triangle");
				isError = true;
				break;
			}
			m->indices.push_back(face.mIndices[0]);
			m->indices.push_back(face.mIndices[1]);
			m->indices.push_back(face.mIndices[2]);
		}

		if (isError && isAllocated) {
			delete m;
		} else {
			ServiceLocator::getMeshLibrary()->addMesh(name, m);
			ret = m;
		}
		return ret;
	}
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

}