#include "GameObject.h"
#include "Component.h"
#include "Mesh.h"
#include "MeshRenderer.h"

namespace Kiwi {

K_COMPONENT_S(MeshRenderer)

MeshRenderer::MeshRenderer() :
	Renderer()
{

}

MeshRenderer::~MeshRenderer() {

}

void MeshRenderer::render() {
	// Check for enabled and MeshFilter
	MeshFilter *meshFilter;
	if (!enabled ||
		(meshFilter = Component::gameObject->GetComponent<MeshFilter>()) == nullptr) {
		return;
	}

	// MeshFilter contains a reference to a mesh
	// which contains an already setup VAO with VBOs

	material->shader.useProgram();
	glBindVertexArray(meshFilter->mesh->VAO);
	glDrawElements(GL_TRIANGLES, meshFilter->mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

}