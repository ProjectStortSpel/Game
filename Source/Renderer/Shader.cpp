#include "Shader.h"

using namespace Renderer;

void Shader::InitShaderProgram()
{
	/* CREATE THE PROGRAM */
	m_shaderProg = glCreateProgram();
}

bool Shader::AddShader(const char* source_file, GLenum shader_type)
{
	// load file into string
	std::ifstream file;
	file.open(source_file);
	if (!file.is_open())
	{
		std::printf("ERROR creating opening shader file %s\n", source_file);
		return false;
	}
	std::string shaderString; // string to load shader into
	char line[256];
	while (!file.eof())
	{
		memcpy(line, "", 256);
		file.getline(line, 256);
		shaderString += line;
		shaderString += '\n';
	}
	file.close();

	GLuint shader;
	/* CREATING THE SHADER */
	shader = glCreateShader(shader_type);

	// load source from a char array
	const char* source = shaderString.c_str();
	/* ATTACH THE SOURCE TO THE SHADER */
	glShaderSource(shader, 1, &source, NULL);
	/* COMPILE SHADER */
	glCompileShader(shader);

	/* ERROR CHECKING */
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		/* GET THE SIZE OF THE ERROR LOG */
		GLint logSize = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

		/* GET THE ERROR LOG */
		GLchar *errorLog;
		errorLog = new GLchar[logSize];
		glGetShaderInfoLog(shader, logSize, &logSize, &errorLog[0]);

		/* PRINT THE ERROR LOG */
		for (int i = 0; i < logSize; i++)
		{
			std::printf("%c", errorLog[i]);
		}
		std::printf("\n");

		/* DO SOME CLEANING :) */
		delete[](errorLog);
		glDeleteShader(shader);

		return false;
	}

	/* ATTACH SHADER TO THE SHADER PROGRAM*/
	glAttachShader(m_shaderProg, shader);

	/* STORE THE SHADER SO WE CAN DETACH/DELETE IT LATER */
	m_shaders.push_back(shader);

	return true;
	/* TODO: WE SHOULD ADD ERROR HANDELING HERE LATER! */
}

bool Shader::FinalizeShaderProgram()
{
	/* LINK THE PROGRAM */
	glLinkProgram(m_shaderProg);

	/* ERROR HANDELING */
	GLint success = 0;
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, (int*)&success);

	if (success == GL_FALSE)
	{
		/* GET THE SIZE OF THE ERROR LOG */
		GLint logSize = 0;
		glGetProgramiv(m_shaderProg, GL_INFO_LOG_LENGTH, &logSize);

		/* GET THE ERROR LOG*/
		GLchar *errorLog;
		errorLog = new GLchar[logSize];
		glGetProgramInfoLog(m_shaderProg, logSize, &logSize, &errorLog[0]);

		/* PRINT THE ERROR LOG */
		for (int i = 0; i < logSize; i++)
		{
			printf("%c", errorLog[i]);
		}
		printf("\n");

		/* DO SOME CLEANING :) */
		delete[](errorLog);

		/* REMOVE PROGRAM */
		glDeleteProgram(m_shaderProg);

		/* REMOVE SHADERS */
		for (int i = 0; i < m_shaders.size(); i++)
		{
			glDeleteShader(m_shaders[i]);
		}
		m_shaders.clear();
		return false;
	}
	printf("Created Shader Program : %i\n", m_shaderProg);
	/* DETACH THE SHADERS NOW WHEN THE PROGRAM IS LINKED */
	for (int i = 0; i < m_shaders.size(); i++)
	{
		glDetachShader(m_shaderProg, m_shaders[i]);
	}
	m_shaders.clear();
	return true;
}

void Shader::UseProgram()
{
	/* USE PROGRAM */
	glUseProgram(m_shaderProg);
}

bool Shader::SetUniformBuffer(GLuint shader_binding, GLuint data, int size)
{
	//glGetProgramResourceIndex​( m_Program, GL_SHADER_STORAGE_BLOCK​​, const char *name​ );
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, shader_binding, data, 0, size);

	return true;
}

bool Shader::SetUniVariable(const char* p_Name, VariableTyp p_Typ, void* p_Value)
{
	// FIXA PREFETCH MED STD MAP
	glUseProgram(this->m_shaderProg);

	GLint location = glGetUniformLocation(m_shaderProg, p_Name);

	if (location != -1)
	{
		const float* value = (GLfloat*)p_Value;

		switch (p_Typ)
		{
		case mat4x4:
			glUniformMatrix4fv(location, 1, GL_FALSE, value);
			return true;
			break;
		case mat3x3:
			glUniformMatrix3fv(location, 1, GL_FALSE, value);
			return true;
			break;
		case vector3:
			glUniform3f(location, value[0], value[1], value[2]);
			return true;
			break;
		case vector4:
			glUniform4f(location, value[0], value[1], value[2], value[3]);
			return true;
			break;
		case glfloat:
			glUniform1f(location, *value);
			return true;
			break;
		case glint:
			glUniform1i(location, *(int*)p_Value);
			return true;
			break;
		default:
			printf("Dont support given type for variable '%s'\n", p_Name);
			return false;
			break;
		}
	}

	printf("Can't find Variable named '%s'\n", p_Name);
	return false;
}

void Shader::CheckUniformLocation(const char* _uniformName, int _unitLocation)
{
	this->UseProgram();
	int location = glGetUniformLocation(m_shaderProg, _uniformName);
	if (location < 0)
		std::cout << "Error: Uniform sampler2D '" << _uniformName << "' not found/set." << std::endl;
	else
		glUniform1i(location, _unitLocation);
}

GLuint Shader::GetShaderProgram()
{
	return m_shaderProg;
}
