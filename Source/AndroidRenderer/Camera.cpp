#include "Camera.h"

Camera::Camera(int windowWidth, int windowHeight)
{
	m_pos = glm::vec3( 8, 2.5, 7 );
	m_up = vec3(0, 0, -1);
	m_right = vec3(1, 0, 0);
	m_look = vec3(0, -1, 0);

	m_camPitch = -M_PI/2.0f;
	m_camYaw = M_PI;
	m_sensitivity = 0.002f;
	m_moveSpeed = 5.0f;

	m_farPlane = 100.f;

	m_projectionMatrix = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.15f, m_farPlane);
	
	doLerp = false;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	if (doLerp)
	{
		float t = m_timer / m_time;

		t = t * t * t * (t * (6 * t - 15) + 10); //SMOOTH AS FUCK

		m_pos = lerp(m_startP, m_targetP, t);
		m_right = slerp(m_startR, m_targetR, t);
		m_up = slerp(m_startU, m_targetU, t);
		m_look = slerp(m_startL, m_targetL, t);
		m_timer += dt;
		if (m_timer > m_time)
		{
			doLerp = false;
			m_pos = m_targetP;
			m_right = m_targetR;
			m_up = m_targetU;
			m_look = m_targetL;
		}
	}
}

vec3 Camera::lerp(vec3 start, vec3 end, float percent)
{
	return (start + (end - start)*percent);
}

vec3 Camera::slerp(vec3 start, vec3 end, float percent)
{
	return (1 - percent)*start + end*percent;
}


void Camera::MoveToAndLookAt(vec3 p_Pos, vec3 p_Up, vec3 p_Target, float p_Time)
{
	m_targetP = vec3(0.0f, 0.0f, 0.0f);
	m_targetR = vec3(1.0f, 0.0f, 0.0f);
	m_targetU = p_Up;
	m_targetL = vec3(0.0f, 0.0f, 1.0f);

	m_startP = m_pos;
	m_startU = m_up;
	m_startR = m_right;
	m_startL = m_look;

	m_targetP = p_Pos;

	m_targetL = glm::normalize(p_Target - m_targetP);
	m_targetR = glm::normalize(glm::cross(m_targetU, m_targetL));
	m_targetU = glm::cross(m_targetL, m_targetR);

	m_time = p_Time;
	m_timer = 0;
	doLerp = true;
}

void Camera::MoveForward(float dt)
{
	m_pos += m_moveSpeed*dt*m_look;
}

void Camera::MoveBackward(float dt)
{
	m_pos -= m_moveSpeed*dt*m_look;
}

void Camera::MoveLeft(float dt)
{
	m_pos -= m_moveSpeed*dt*m_right;
}

void Camera::MoveRight(float dt)
{
	m_pos += m_moveSpeed*dt*m_right;
}

void Camera::UpdateMouse(float midX, float midY, int x, int y)
{
	m_camYaw += m_sensitivity*(midX - (float)x);
	m_camPitch += m_sensitivity*(midY - (float)y);

	m_look = vec3(cos(m_camPitch) * sin(m_camYaw), sin(m_camPitch), cos(m_camPitch)*cos(m_camYaw));
	m_right = vec3(sin(m_camYaw - M_PI / 2.0f), 0, cos(m_camYaw - M_PI / 2.0f));
	m_up = glm::cross(m_right, m_look);
}

void Camera::UpdateTouch(float dx, float dy)
{
	m_camYaw += m_sensitivity*dx;
	m_camPitch += m_sensitivity*dy;

	m_look = vec3(cos(m_camPitch) * sin(m_camYaw), sin(m_camPitch), cos(m_camPitch)*cos(m_camYaw));
	m_right = vec3(sin(m_camYaw - M_PI / 2.0f), 0, cos(m_camYaw - M_PI / 2.0f));
	m_up = glm::cross(m_right, m_look);
}

mat4* Camera::GetViewMatrix()
{
	//if (m_pos.y > 1.2)
		//m_pos -= vec3(0.0, 0.025, -0.002);

	m_viewMatrix = glm::lookAt(m_pos, m_pos + m_look, m_up);

	return &m_viewMatrix;
}

void Camera::UpdateProjMatrix(int windowWidth, int windowHeight)
{
	m_projectionMatrix = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.15f, m_farPlane);
}