#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <GL/glew.h>
#include "util.h"
#include "Object.h"

namespace Kiwi {

enum class ShaderType {
	VERTEX_SHADER = GL_VERTEX_SHADER,
	TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
	TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
	GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
	COMPUTE_SHADER = GL_COMPUTE_SHADER
};

class ShaderProgram;
using ShaderLibrary = std::unordered_map<String, ShaderProgram *>;

/**
 * A struct containing the location and the type of
 * this variable
 */
struct glslVarInfo {
	GLint location;
	GLenum type;
};

// A map containing names of uniform variables to their info
using UniformMap = std::unordered_map<String, glslVarInfo>;

/**
 * Encapsulates data about an OpenGL program.
 */
class ShaderProgram : public Object {
public:
	/**
	 * Initializes a ShaderProgram and adds it to the
	 * global shader library.
	 */
	ShaderProgram(const String &name) :
		Object(name), shaderName(name),
		shaderHandles { 0, 0, 0, 0, 0, 0 },
		programHandle(0)
	{
		getShaderLibrary().emplace(name, this);
	}

	// Copy construct and copy assignment are disabled
	ShaderProgram(const ShaderProgram &other) = delete;
	ShaderProgram &operator=(ShaderProgram other) = delete;

	/**
	 * Destructs a shader program. If the program is linked,
	 * all shaders are detached. All shaders are deleted. Then,
	 * the program is deleted.
	 */
	~ShaderProgram();

	/**
	 * Loads a shader located at the path specified by \p shaderPath
	 * with type specified by \p shaderType, and compiles it.
	 * 
	 * If a shader of the requested type already exists for this
	 * ShaderProgram, the flag \p force indicates whether or not to
	 * preserve the already existing shader. If \p force is set to true,
	 * the existing shader will be detached from the program (if the
	 * shader is attached to the program) and then deleted.
	 *
	 * This function can fail if any OpenGL function calls fail or if
	 * the provided arguments are invalid (such as the requested \p shaderType
	 * being invalid, or the shader file not being able to be read).
	 *
	 * In the case of an error, either a std::runtime_exception or an
	 * std::invalid_argument is thrown. The error is also logged in the
	 * rendering and default loggers.
	 *
	 * @param shaderType can be one of
	 *                   \p ShaderType::VERTEX_SHADER,
	 *                   \p ShaderType::TESS_CONTROL_SHADER,
	 *                   \p ShaderType::TESS_EVALUATION_SHADER,
	 *                   \p ShaderType::GEOMETRY_SHADER,
	 *                   \p ShaderType::FRAGMENT_SHADER,
	 *                   \p ShaderType::COMPUTE_SHADER
	 * @param shaderPath the path to the file containing the shader code
	 * @param force      boolean flag to force loading of the shader even
	 *                   if one already exists
	 *
	 * @throw std::invalid_argument if the \p shaderType is invalid or the
	 *        						file \p shaderPath cannot be read
	 * @throw std::runtime_error    if an OpenGL call fails, such as through
	 *        						compilation failure
	 */
	void loadShader(ShaderType shaderType, const String &shaderPath,
					bool force = false);

	/**
	 * Creates a shader program with the loaded shaders attached. If
	 * a compute shader is loaded, then all other shaders are ignored.
	 * Otherwise, all shaders are attached to this program.
	 *
	 * This function also loads all the uniforms and attributes
	 * of this program into the appropriate uniform and attribute map.
	 * These map the name of the uniform/attribute to a glslVarInfo
	 * struct containing the location of the variable and the type
	 * of the variable.
	 *
	 * @throw std::runtime_error if an OpenGL call fails, such as through
	 *        					 linking failure
	 */
	void linkProgram();

	/**
	 * Use this program in the current OpenGL rendering context.
	 * If this program is not already linked, then an std::runtime_error
	 * is thrown.
	 *
	 * @throw std::runtime_error if the program is not linked
	 */
	void useProgram();

	template <typename T>
	void setUniform(String name, T val);

	GLuint getAttribLocation(const String &name);

	static ShaderLibrary &getShaderLibrary() {
		static ShaderLibrary shaderLib;
		return shaderLib;
	}

private:
	static int shaderType2Int(GLenum shaderType) {
		switch (shaderType) {
		case GL_VERTEX_SHADER: return 0;
		case GL_TESS_CONTROL_SHADER: return 1;
		case GL_TESS_EVALUATION_SHADER: return 2;
		case GL_GEOMETRY_SHADER: return 3;
		case GL_FRAGMENT_SHADER: return 4;
		case GL_COMPUTE_SHADER: return 5;
		default: return -1;
		}
	}

	static int shaderType2Int(ShaderType shaderType) {
		switch (shaderType) {
		case ShaderType::VERTEX_SHADER: return 0;
		case ShaderType::TESS_CONTROL_SHADER: return 1;
		case ShaderType::TESS_EVALUATION_SHADER: return 2;
		case ShaderType::GEOMETRY_SHADER: return 3;
		case ShaderType::FRAGMENT_SHADER: return 4;
		case ShaderType::COMPUTE_SHADER: return 5;
		default: return -1;
		}
	}

	static GLenum convertShaderType(ShaderType shaderType) {
		switch (shaderType) {
		case ShaderType::VERTEX_SHADER: return GL_VERTEX_SHADER;
		case ShaderType::TESS_CONTROL_SHADER: return GL_TESS_CONTROL_SHADER;
		case ShaderType::TESS_EVALUATION_SHADER: return GL_TESS_EVALUATION_SHADER;
		case ShaderType::GEOMETRY_SHADER: return GL_GEOMETRY_SHADER;
		case ShaderType::FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
		case ShaderType::COMPUTE_SHADER: return GL_COMPUTE_SHADER;
		}
	}

	static const String shaderType2Str(GLenum shaderType) {
		switch (shaderType) {
		case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
		case GL_TESS_CONTROL_SHADER: return "GL_TESS_CONTROL_SHADER";
		case GL_TESS_EVALUATION_SHADER: return "GL_TESS_EVALUATION_SHADER";
		case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
		default: return "";
		}
	}
	
	String shaderName;
	GLuint shaderHandles[6];
	GLuint programHandle;

	UniformMap uniforms;
};

}

#endif // __SHADER_PROGRAM_H__
