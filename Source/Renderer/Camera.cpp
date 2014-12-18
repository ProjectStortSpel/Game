#include "Camera.h"

Camera::Camera(int windowWidth, int windowHeight)
{
	m_pos = glm::vec3( 7, 13, 7 );
	m_up = vec3(0, 0, -1);
	m_right = vec3(1, 0, 0);
	m_look = vec3(0, -1, 0);

	m_camPitch = -M_PI/2.0f;
	m_camYaw = M_PI;
	m_sensitivity = 0.002f;
	m_moveSpeed = 1.0f;

	m_farPlane = 100.f;

	m_projectionMatrix = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.15f, m_farPlane);
}

Camera::Camera()
{
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

void Camera::UpdateMouse(float midX, float midY, int x, int y)
{
	m_camYaw += m_sensitivity*(midX - (float)x);
	m_camPitch += m_sensitivity*(midY - (float)y);

	m_look = vec3(cos(m_camPitch) * sin(m_camYaw), sin(m_camPitch), cos(m_camPitch)*cos(m_camYaw));
	m_right = vec3(sin(m_camYaw - M_PI / 2.0f), 0, cos(m_camYaw - M_PI / 2.0f));
	m_up = glm::cross(m_right, m_look);
}

mat4* Camera::GetViewMatrix()
{
	m_viewMatrix = glm::lookAt(m_pos, m_pos + m_look, m_up);

	return &m_viewMatrix;
}

void Camera::UpdateProjMatrix(int windowWidth, int windowHeight)
{
	m_projectionMatrix = glm::perspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.f);
}