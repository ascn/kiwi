#include <GL/glew.h>
#include <sstream>
#include <fstream>

#include "ShaderProgram.h"
#include "ServiceLocator.h"

namespace Kiwi {

ShaderProgram::~ShaderProgram() {
	if (programHandle != 0) {
		GLsizei count;
		GLuint attachedShaders[6];
		glGetAttachedShaders(programHandle, 6, &count, attachedShaders);
		for (int i = 0; i < count; ++i) {
			glDetachShader(programHandle, attachedShaders[i]);
		}
	}

	for (int i = 0; i < 6; ++i) {
		if (shaderHandles[i] != 0) {
			glDeleteShader(shaderHandles[i]);
		}
	}

	glDeleteProgram(programHandle);
}

void ShaderProgram::loadShader(ShaderType type, const String &shaderPath, bool force) {
	// Check for invalid shader type
	GLenum shaderType = convertShaderType(type);
	int shaderIdx = shaderType2Int(shaderType);
	if (shaderIdx < 0) {
		ServiceLocator::getLogger(LOG_RENDERING).Log(
			"Invalid shader type in loading of shader " + shaderName, LOG_LEVEL::err
		);
		throw std::invalid_argument("invalid shaderType");
	}
	
	// Check if not forcing and shader exists
	if (!force && shaderHandles[shaderIdx] != 0) {
		String warn = shaderName;
		shaderName += " already has a ";
		shaderName += shaderType2Str(shaderType);
		shaderName += " loaded, use force = true to replace";
		ServiceLocator::getLogger(LOG_RENDERING).Log(warn, LOG_LEVEL::warn);
		return;
	}

	// Create shader
	GLuint shaderHandle;
	if (!(shaderHandle = glCreateShader(shaderType))) {
		String err = "Could not create ";
		err += shaderType2Str(shaderType);
		err += " in loading of shader ";
		err += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(err, LOG_LEVEL::err);
		throw std::runtime_error("could not create vertex shader");
	}

	// Read in shader
	String shaderCode;
	std::ifstream shaderStream(shaderPath, std::ios::in);
	if (shaderStream.is_open()) {
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
		String info = "Successfully read ";
		info += shaderType2Str(shaderType);
		info += " ";
		info += shaderPath;
		info += " in loading of shader ";
		info += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(info, LOG_LEVEL::info);
	} else {
		String err = "Could not read ";
		err += shaderType2Str(shaderType);
		err += " ";
		err += shaderPath;
		err += " in loading of shader ";
		err += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(err, LOG_LEVEL::err);
		glDeleteShader(shaderHandle);
		throw std::invalid_argument("could not read shader " + shaderPath);
	}

	// Compile shader
	char const *shaderSourcePointer = shaderCode.c_str();
	glShaderSource(shaderHandle, 1, &shaderSourcePointer, NULL);
	glCompileShader(shaderHandle);

	// Check shader compilation
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (result == GL_FALSE) { /* Error in compiling */
		String err = "Compilation failure of ";
		err += shaderType2Str(shaderType);
		err += " ";
		err += shaderPath;
		err += " in loading of shader ";
		err += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(err, LOG_LEVEL::err);
		if (infoLogLength > 0) {
			std::vector<char> shaderErrMsg(infoLogLength + 1);
			glGetShaderInfoLog(shaderHandle, infoLogLength, NULL, &shaderErrMsg[0]);
			ServiceLocator::getLogger(LOG_RENDERING).Log(&shaderErrMsg[0]);
		}
		glDeleteShader(shaderHandle);
		throw std::runtime_error("could not compile shader " + shaderPath);
	} else {
		String info = "Successfully compiled ";
		info += shaderType2Str(shaderType);
		info += " ";
		info += " in loading of shader ";
		info += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(info, LOG_LEVEL::info);
	}
	
	// Check if shader is already loaded
	if (shaderHandles[shaderIdx] != 0) {
		// Delete the shader already loaded
		if (programHandle != 0) {
			GLsizei count;
			GLuint attachedShaders[6];
			glGetAttachedShaders(programHandle, 6, &count, attachedShaders);
			for (int i = 0; i < count; ++i) {
				if (shaderHandles[shaderIdx] == attachedShaders[i]) {
					glDetachShader(programHandle, shaderHandles[shaderIdx]);
				}
			}
		}
		glDeleteShader(shaderHandles[shaderIdx]);
		shaderHandles[shaderIdx] = 0;
	}

	shaderHandles[shaderIdx] = shaderHandle;
}

void ShaderProgram::linkProgram() {
	// If compute shader exists, then only attach that
	// Otherwise attach all others
	GLuint newProgram;
	if (!(newProgram = glCreateProgram())) {
		String err = "Could not create program in linking of shader ";
		err += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(err, LOG_LEVEL::err);
		throw std::runtime_error("could not create program");
	}

	if (shaderHandles[shaderType2Int(GL_COMPUTE_SHADER)] != 0) {
		glAttachShader(newProgram, shaderHandles[shaderType2Int(GL_COMPUTE_SHADER)]);
	} else {
		for (int i = 0; i < 5; ++i) {
			if (shaderHandles[i] != 0) {
				glAttachShader(newProgram, shaderHandles[i]);
			}
		}
	}

	glLinkProgram(newProgram);

	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(newProgram, GL_LINK_STATUS, &result);
	glGetProgramiv(newProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (result == GL_FALSE) { /* Error in linking */
		String err = "Linking failure of program in linking of shader program ";
		err += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(err, LOG_LEVEL::err);
		if (infoLogLength > 0) {
			std::vector<char> programErrMsg(infoLogLength + 1);
			glGetProgramInfoLog(newProgram, infoLogLength, NULL, &programErrMsg[0]);
			ServiceLocator::getLogger(LOG_RENDERING).Log(&programErrMsg[0]);
		}
		glDeleteProgram(newProgram);
		throw std::runtime_error("could not link shader " + shaderName);
	} else {
		String info = "Successfully linked shader program ";
		info += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(info, LOG_LEVEL::info);
	}

	programHandle = newProgram;

	// Get uniforms and attributes
	GLint count;
	GLint location;
	GLenum type;

	const GLsizei bufSize = 16;
	GLchar name[bufSize];
	GLsizei length;

	// Map uniforms to their location and type
	glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; ++i) {
		glGetActiveUniform(programHandle, static_cast<GLuint>(i), bufSize, &length, NULL, &type, name);
		location = glGetUniformLocation(programHandle, name);

		uniforms.emplace(String(name), glslVarInfo{ location, type });
	}

	glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES, &count);
	for (int i = 0; i < count; ++i) {
		//glGetActiveAttrib(programHandle, static_cast<GLuint>(i), bufSize, &length, NULL, &type, name);
		//location = glGetAttribLocation(programHandle, name);
	}
}

void ShaderProgram::useProgram() {
	if (programHandle == 0) {
		throw std::runtime_error("program not linked");
	}
	glUseProgram(programHandle);
}

template <typename T>
void ShaderProgram::setUniform(String name, T val) {
	glslVarInfo unifInfo;
	try {
		unifInfo = uniforms.at(name);
	} catch (std::exception &) {
		ServiceLocator::getLogger(LOG_RENDERING).Log(
			"invalid uniform name in setUniform", LOG_LEVEL::err);
		return;
	}
	switch (unifInfo.type) {
	case GL_FLOAT:
		if (std::is_same<T, float>::value) {
			glProgramUniform1f(programHandle, unifInfo.location, val);
		}
		return;
	case GL_FLOAT_VEC2:
		if (std::is_same<T, glm::vec2>::value) {
			glProgramUniform2f(programHandle, unifInfo.location, val[0], val[1]);
		}
	}
}

GLuint ShaderProgram::getAttribLocation(const String &name) {
	if (programHandle == 0) {
		throw std::runtime_error("program is not linked");
	}
	GLint ret = glGetAttribLocation(programHandle, name.c_str());
	if (ret == -1) {
		throw std::runtime_error("attribute does not exist");
	}
	return ret;
}

}