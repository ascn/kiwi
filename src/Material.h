#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <GL/glew.h>
#include "Object.h"
#include "ShaderProgram.h"

namespace Kiwi {

class Material : public Object {
public:
	Material(ShaderProgram &s) :
		shader(s) {}

	ShaderProgram &shader;
};

}

#endif // __MATERIAL_H__