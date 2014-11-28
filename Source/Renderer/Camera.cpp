#include "Camera.h"
float m_mouseX, m_mouseY;
using glm::vec3;

Camera::Camera()
{
	m_pos = glm::vec3( 0, 0, 3 );
	m_up = vec3(0, 1, 0);
	m_right = vec3(1, 0, 0);
	m_look = vec3(0, 0, -1);

	m_camPitch = 0.0f;
	m_camYaw = 180.0f;
	m_sensitivity = 0.3f;
	m_moveSpeed = 1.0f;
}


Camera::~Camera()
{
}

void Camera::Update(float midX, float midY, float dt)
{
	//get the mouse x,y in window
	
//	camYaw += sensitivity*(midX - mouseX)*dt;
//	camPitch += sensitivity*(midY - mouseY)*dt;

	float pitch = m_camPitch * (M_PI / 180.f);
	float yaw = m_camYaw * (M_PI / 180.f);
	
	m_look = vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch)*cos(yaw));
	m_right = vec3(sin(yaw - M_PI / 2.0f), 0, cos(yaw - M_PI / 2.0f));
	m_up = glm::cross(m_right, m_look);

//	right = -vec3( cosYaw, 0, -sinYaw );							//right
//	up = vec3(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);	//up
//	look = vec3(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);	//forward

//	printf("right : (%f,%f,%f)\n", right.x, right.y, right.z);
//	printf("look : (%f,%f,%f)\n\n", look.x, look.y, look.z);
//	printf("pos : (%f,%f,%f)\n", pos.x, pos.y, pos.z);
//	printf("Pitch = %f        Yaw = %f \n", camPitch, camYaw);
	
}

void Camera::MoveForward(float dt)
{
	//pos += look*0.1f*moveSpeed;
	m_pos.z -= 0.2f*m_moveSpeed*dt;
}

void Camera::MoveBackward(float dt)
{
	//pos -= look*0.1f*moveSpeed;
	m_pos.z += 0.2f*m_moveSpeed*dt;
}

void Camera::MoveLeft(float dt)
{
	//pos -= right*0.1f*moveSpeed;
	m_pos.x -= 0.2f*m_moveSpeed*dt;
}

void Camera::MoveRight(float dt)
{
	//pos += right*0.1f*moveSpeed;
	m_pos.x += 0.2f*m_moveSpeed*dt;
}

// Pitch should be in the range of [-90 ... 90] degrees and yaw
// should be in the range of [0 ... 360] degrees.
mat4 Camera::GetViewMatrix()//vec3 eye)
{
	////to radians
	//float pitch = camPitch * (M_PI / 180.f);
	//float yaw = camYaw * (M_PI / 180.f);
	//
	//// If the pitch and yaw angles are in degrees,
	//// they need to be converted to radians. Here
	//// I assume the values are already converted to radians.
	//float cosPitch = cos(pitch);
	//float sinPitch = sin(pitch);
	//float cosYaw = cos(yaw);
	//float sinYaw = sin(yaw);

	//right = vec3( cosYaw, 0, -sinYaw );							//right
	//up = vec3(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);	//up
	//look = vec3(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);	//forward

	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	mat4 viewMatrix = mat4(
		vec4(m_right.x, m_up.x, m_look.x, 0),	
		vec4(m_right.y, m_up.y, m_look.y, 0),	
		vec4(m_right.z, m_up.z, m_look.z, 0),	
		vec4(-glm::dot(m_right, m_pos), -glm::dot(m_up, m_pos), -glm::dot(m_look, m_pos), 1)
	);

	return viewMatrix;
}