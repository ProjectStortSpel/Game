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
	ifstream file;
	file.open(source_file);
	if (!file.is_open())
	{
		std::printf("ERROR creating opening shader file %s\n", source_file);
		return false;
	}
	string shaderString; // string to load shader into
	char line[256];
	while (!file.eof())
	{
		strcpy_s(line, "");
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
		delete(errorLog);
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
		delete(errorLog);

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