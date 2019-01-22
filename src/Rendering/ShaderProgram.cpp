#include <GL/glew.h>
#include <sstream>
#include <fstream>

#include "Rendering/ShaderProgram.h"
#include "Core/ServiceLocator.h"

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

	glGetProgramInterfaceiv(programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &count);
	const GLenum unifProperties[4] = { GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };
	for (int i = 0; i < count; ++i) {
		GLint values[4];
		glGetProgramResourceiv(programHandle, GL_UNIFORM, i, 4, unifProperties, 4, NULL, values);
		if (values[0] != -1) { continue; }
		std::vector<char> nameData(values[2]);
		glGetProgramResourceName(programHandle, GL_UNIFORM, i,
								 static_cast<GLsizei>(nameData.size()),
								 NULL, &nameData[0]);
		uniforms.emplace(String(nameData.begin(), nameData.end() - 1),
						 glslVarInfo{ values[3], static_cast<GLenum>(values[1]) });
	}

	glGetProgramInterfaceiv(programHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &count);
	const GLenum attribProperties[] = { GL_TYPE, GL_NAME_LENGTH, GL_LOCATION };
	for (int i = 0; i < count; ++i) {
		GLint values[3];
		glGetProgramResourceiv(programHandle, GL_PROGRAM_INPUT, i, 3, attribProperties, 3, NULL, values);
		std::vector<char> nameData(values[1]);
		glGetProgramResourceName(programHandle, GL_PROGRAM_INPUT, i,
								 static_cast<GLsizei>(nameData.size()),
								 NULL, &nameData[0]);
		attribs.emplace(String(nameData.begin(), nameData.end() - 1),
						 glslVarInfo { values[2], static_cast<GLenum>(values[0]) });
	}
}

void ShaderProgram::loadAndLink(const String &vertPath,
								const String &tessControlPath,
								const String &tessEvalPath,
								const String &geoPath,
								const String &fragPath) {
	if (vertPath != "") { loadShader(ShaderType::VERTEX_SHADER, vertPath); }
	if (tessControlPath != "") { loadShader(ShaderType::TESS_CONTROL_SHADER, tessControlPath); }
	if (tessEvalPath != "") { loadShader(ShaderType::TESS_EVALUATION_SHADER, tessEvalPath); }
	if (geoPath != "") { loadShader(ShaderType::GEOMETRY_SHADER, geoPath); }
	if (fragPath != "") { loadShader(ShaderType::FRAGMENT_SHADER, fragPath); }

	linkProgram();
}

void ShaderProgram::setUniformHelper(String name, std::vector<GLenum> types, glslVarInfo *unifInfo) {
	try {
		*unifInfo = uniforms.at(name);
	} catch (std::exception &) {
		String err = "invalid uniform name ";
		err += name;
		err += " in setUniform of shader ";
		err += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(
			err, LOG_LEVEL::err);
		throw std::runtime_error(err);
	}
	bool anyMatch = false;
	for (const auto &t : types) {
		if (unifInfo->type == t) {
			anyMatch = true;
			break;
		}
	}
	if (!anyMatch) {
		String err = "cannot set uniform ";
		err += name;
		err += " due to incorrect type in shader ";
		err += shaderName;
		ServiceLocator::getLogger(LOG_RENDERING).Log(
			"cannot set uniform " + name + " due to incorrect type", LOG_LEVEL::err);
		throw std::runtime_error("cannot set uniform " + name + " due to incorrect type");

	}
	return;
}

void ShaderProgram::setUniform(String name, float val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_FLOAT }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniform1f(programHandle, unifInfo.location, val);
}

void ShaderProgram::setUniform(String name, Vector2 val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_FLOAT_VEC2 }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniform2f(programHandle, unifInfo.location, val[0], val[1]);
}

void ShaderProgram::setUniform(String name, Vector3 val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_FLOAT_VEC3 }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniform3f(programHandle, unifInfo.location, val[0], val[1], val[2]);
}

void ShaderProgram::setUniform(String name, Vector4 val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_FLOAT_VEC4 }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniform4f(programHandle, unifInfo.location, val[0], val[1], val[2], val[3]);
}

void ShaderProgram::setUniform(String name, int val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_INT, GL_SAMPLER_2D }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniform1i(programHandle, unifInfo.location, val);
}

void ShaderProgram::setUniform(String name, IVector2 val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_INT_VEC2 }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniform2i(programHandle, unifInfo.location, val[0], val[1]);
}

void ShaderProgram::setUniform(String name, IVector3 val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_INT_VEC3 }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniform3i(programHandle, unifInfo.location, val[0], val[1], val[2]);
}

void ShaderProgram::setUniform(String name, IVector4 val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_INT_VEC4 }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniform4i(programHandle, unifInfo.location, val[0], val[1], val[2], val[3]);
}

void ShaderProgram::setUniform(String name, Matrix4 val) {
	glslVarInfo unifInfo;
	try {
		setUniformHelper(name, { GL_FLOAT_MAT4 }, &unifInfo);
	} catch (std::exception &) {
		return;
	}
	glProgramUniformMatrix4fv(programHandle, unifInfo.location, 1, GL_FALSE, &val[0][0]);
}

void ShaderProgram::useProgram() {
	if (programHandle == 0) {
		throw std::runtime_error("program not linked");
	}
	glUseProgram(programHandle);
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