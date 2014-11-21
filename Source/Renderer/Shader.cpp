#include "Shader.h"

using namespace Renderer;

bool Shader::SetUniVariable(const char* p_Name, VariableTyp p_Typ, void* p_Value)
{
	// FIXA PREFETCH MED STD MAP


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
		case sampler2D:
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *(GLuint*)p_Value);
			glUniform1i(location, 0);
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

GLuint Shader::Init(ShaderInfo p_ShaderInfo[3])
{
	GLuint shader[3];
	GLuint program = glCreateProgram();

	if (program == 0)
	{
		printf("ERROR: Failed to create GL shader program\n");
		return false;
	};

	// compile the shaders
	for (int i = 0; i < 3; i++)
	{
		if (p_ShaderInfo[i].type != GL_NONE)
		{
			printf(p_ShaderInfo[i].file);
			printf("\n");
			CompileShader(p_ShaderInfo[i].file, p_ShaderInfo[i].type, shader[i]);
		}
	}

	// attatch the shaders to the program
	for (int i = 0; i < 3; i++)
	{
		if (p_ShaderInfo[i].type != GL_NONE)
			glAttachShader(program, shader[i]);
	}

	// link program
	glLinkProgram(program);

	// verify the link
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		printf("ERROR: Failed to link GL shader program\n");
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
		GLchar* infoLog = (GLchar *)malloc(maxLength);
		glGetProgramInfoLog(program, maxLength, NULL, infoLog);
		printf("Shader InfoLog:\n%s\n\n", infoLog);
		return false;
	};
	printf("Created Shader Program %i\n", program);
	m_shaderProg = program;
	return program;
}

GLuint Shader::GetShaderProgram()
{
	return m_shaderProg;
}

bool Shader::CompileShader(const char* p_ShaderFile, GLenum& p_Type, GLuint& p_Shader)
{

	ifstream fileStream;
	fileStream.open(p_ShaderFile);
	if (!fileStream.is_open())
	{
		printf("ERROR: Failed to open GL shader file %s\n", p_ShaderFile);
		return false;
	}

	string shaderString;
	char line[256];

	while (!fileStream.eof())
	{
		fileStream.getline(line, 256);
		shaderString += line;
		shaderString += "\n";
	}
	fileStream.close();

	// create the shader
	p_Shader = glCreateShader(p_Type);
	if (p_Shader == 0)
	{
		printf("ERROR: Failed to create GL shader of type %i\n", p_Type);
		return false;
	}

	const char* sourcePtr = shaderString.c_str();
	glShaderSource(p_Shader, 1, &sourcePtr, NULL);
	glCompileShader(p_Shader);

	// check for errors
	int result = 0;

	glGetShaderiv(p_Shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		printf("ERROR: Failed to compile GL shader of type %i\n", p_Type);

		int length = 0;
		glGetShaderiv(p_Shader, GL_INFO_LOG_LENGTH, &length);
		if (length > 0){
			//Create error log
			char* errLog = new char[length];
			int written = 0;
			glGetShaderInfoLog(p_Shader, length, &written, errLog);
			printf("GL shader error log;\n%s\n", errLog);
			delete[] errLog;
		}
		return false;
	}
	printf("Created Shader %i\n", p_Shader);
	return true;
}