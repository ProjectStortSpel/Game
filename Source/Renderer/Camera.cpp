#include "Camera.h"

Camera::Camera()
{
	m_pos = glm::vec3( 0, 0, 4 );
	m_up = vec3(0, 1, 0);
	m_right = vec3(1, 0, 0);
	m_look = vec3(0, 0, -1);

	m_camPitch = 0.0f;
	m_camYaw = M_PI;
	m_sensitivity = 1.2f;
	m_moveSpeed = 1.0f;
}


Camera::~Camera()
{
}

void Camera::MoveForward(float dt)
{
	m_pos += 5.f*m_moveSpeed*dt*m_look;
}

void Camera::MoveBackward(float dt)
{
	m_pos -= 5.f*m_moveSpeed*dt*m_look;
}

void Camera::MoveLeft(float dt)
{
	m_pos -= 5.0f*m_moveSpeed*dt*m_right;
}

void Camera::MoveRight(float dt)
{
	m_pos += 5.0f*m_moveSpeed*dt*m_right;
}

void Camera::UpdateMouse(float midX, float midY, int x, int y, float dt)
{
	m_camYaw += m_sensitivity*(midX - (float)x)*dt;
	m_camPitch += m_sensitivity*(midY - (float)y)*dt;

	m_look = vec3(cos(m_camPitch) * sin(m_camYaw), sin(m_camPitch), cos(m_camPitch)*cos(m_camYaw));
	m_right = vec3(sin(m_camYaw - M_PI / 2.0f), 0, cos(m_camYaw - M_PI / 2.0f));
	m_up = glm::cross(m_right, m_look);
}

mat4* Camera::GetViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_pos, m_pos + m_look, m_up);

	return &m_viewMatrix;
}