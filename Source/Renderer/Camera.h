#pragma once
#include "stdafx.h"

#define M_PI 3.14159265359

using glm::vec3; 
using glm::vec4;
using glm::mat4;

class Camera
{
private:
	vec3 m_right, m_up, m_look, m_pos;
	float m_camPitch, m_camYaw, m_sensitivity, m_moveSpeed;

	glm::mat4 *m_viewMatrix;

public:
	Camera();
	~Camera();

	void Update(float midX, float midY, float dt);
	void MoveForward(float dt);
	void MoveBackward(float dt);
	void MoveLeft(float dt);
	void MoveRight(float dt);

	mat4 GetViewMatrix(); // vec3 eye); /*FPS camera*/

	vec3*	GetRight( ) { return &m_right; }
	vec3*	GetUp() { return &m_up; }
	vec3*	GetLook() { return &m_look; }
	vec3*	GetPos() { return &m_pos; }

};

