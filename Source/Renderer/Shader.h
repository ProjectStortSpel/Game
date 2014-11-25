/*
Author: Anders, Christian
*/
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
		void	InitShaderProgram();
		bool	AddShader(const char* source_file, GLenum shader_type);
		bool	FinalizeShaderProgram();

		void	UseProgram();

		bool	SetUniformBuffer(GLuint shader_binding, GLuint data, int size);

		bool    SetUniVariable(const char* p_variableName, VariableTyp p_variableType, void* p_Value);

		GLuint  GetShaderProgram(void);

	private:
		bool CompileShader(const char* p_ShaderFile, GLenum& p_Type, GLuint& p_Shader);

	private:
		GLuint m_shaderProg;
		std::vector<GLuint>	m_shaders;
	};
}
#endif