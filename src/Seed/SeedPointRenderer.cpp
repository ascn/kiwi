#include "Seed/SeedPointRenderer.h"
#include "Seed/Emitter.h"

namespace Kiwi::Seed {

K_COMPONENT_S(SeedPointRenderer)

SeedPointRenderer::SeedPointRenderer(Scene &s, Emitter *emitter) :
	SeedRenderer(s, emitter)
{
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
}

SeedPointRenderer::~SeedPointRenderer() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void SeedPointRenderer::render(bool useMaterial) {
	if (!enabled || !emitter) { return; }

	if (useMaterial) { material->shader.useProgram(); }

	dataVec.clear();

	// Get position and color vectors of this emitter
	Vector<Vector3> posVec;
	Vector<Vector4> colVec;
	emitter->manager.getActive(PropertyNames::Position, posVec);
	emitter->manager.getActive(PropertyNames::Color, colVec);
	unsigned int numParticles = static_cast<unsigned int>(posVec.size());
	for (unsigned int i = 0; i < numParticles; ++i) {
		dataVec.push_back({ posVec[i], colVec[i] });
	}
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PointRenderData) * dataVec.size(), dataVec.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointRenderData), (void *) 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(PointRenderData), (void *) offsetof(PointRenderData, color));

	glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(dataVec.size()));

	glBindVertexArray(0);
}

}