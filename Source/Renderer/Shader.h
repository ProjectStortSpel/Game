#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"

using namespace std;

namespace Renderer
{
	struct ShaderInfo
	{
		GLenum type;
		const char* file;
	};

	enum VariableTyp
	{
		mat4x4,
		mat3x3,
		vector3,
		vector4,
		glfloat,
		sampler2D
	};

	class Shader
	{
	public:
		bool    SetUniVariable(const char* p_variableName, VariableTyp p_variableType, void* p_Value);

		GLuint	Init(ShaderInfo p_ShaderInfo[3]);
		GLuint  GetShaderProgram(void);

	private:
		bool CompileShader(const char* p_ShaderFile, GLenum& p_Type, GLuint& p_Shader);

	private:
		GLuint m_shaderProg;
	};
}
#endif