#include "Quaternion.h"
#define DELTA 0.0001f
#include <iostream>

Quaternion::Quaternion()
{
	this->m_w = 1;
	this->m_x = 0;
	this->m_y = 0;
	this->m_z = 0;
}

Quaternion Quaternion::operator* (Quaternion q)
{
	Quaternion q1 = *this;
	Quaternion q2 = q;
	Quaternion ret_value;

	ret_value.m_w = 
			q1.m_w*q2.m_w
		-	q1.m_x*q2.m_x
		-	q1.m_y*q2.m_y
		-	q1.m_z*q2.m_z;

	ret_value.m_x = 
			q1.m_w*q2.m_x
		+	q1.m_x*q2.m_w
		+	q1.m_y*q2.m_z
		-	q1.m_z*q2.m_y;;


	ret_value.m_y = 
			q1.m_w*q2.m_y
		-	q1.m_z*q2.m_z
		+	q1.m_y*q2.m_w
		+	q1.m_z*q2.m_z;

	ret_value.m_z = 
			q1.m_w*q2.m_z
		+	q1.m_x*q2.m_y
		-	q1.m_y*q2.m_x
		+	q1.m_z*q2.m_w;

	return ret_value;
}


void Quaternion::Rotate( const glm::vec3 &vector, float w)
{
	/* Stor the data as a Quaternion */
	this->m_w = cosf(w/2);
	this->m_x = vector.x * sinf(w / 2);
	this->m_y = vector.y * sinf(w / 2);
	this->m_z = vector.z * sinf(w / 2);
}

void Quaternion::Normalize()
{
	float size = sqrtf(
			this->m_w*this->m_w 
		+	this->m_x*this->m_x 
		+	this->m_y*this->m_y 
		+	this->m_z*this->m_z);

	this->m_w /= size;
	this->m_x /= size;
	this->m_y /= size;
	this->m_z /= size;
}

glm::mat4 Quaternion::QuaternionToMatrix()
{
	this->Normalize();

	glm::mat4 ret_value;

	float w = this->m_w;
	float x = this->m_x;
	float y = this->m_y;
	float z = this->m_z;

	float wx2, wy2, wz2, xx2, yy2, yz2, xy2, xz2, zz2, x2, y2, z2;

	x2 = x + x; y2 = y + y;
	z2 = z + z;
	xx2 = x * x2; xy2 = x * y2; xz2 = x * z2;
	yy2 = y * y2; yz2 = y * z2; zz2 = z * z2;
	wx2 = w * x2; wy2 = w * y2; wz2 = w * z2;
	
	ret_value[0][0] = 1.0f - (yy2 + zz2);	ret_value[0][1] = xy2 + wz2;			ret_value[0][2] = xz2 - wy2;			ret_value[0][3] = 0.0f;
	ret_value[1][0] = xy2 - wz2;			ret_value[1][1] = 1.0f - (xx2 + zz2);	ret_value[1][2] = yz2 + wx2;			ret_value[1][3] = 0.0f;
	ret_value[2][0] = xz2 + wy2;			ret_value[2][1] = yz2 - wx2;			ret_value[2][2] = 1.0f - (xx2 + yy2);	ret_value[2][3] = 0.0f;
	ret_value[3][0] = 0.0f;					ret_value[3][1] = 0.0f;					ret_value[3][2] = 0.0f;					ret_value[3][3] = 1.0f;

	return ret_value;
}

void Quaternion::MatrixToQuaternion(const glm::mat4 &rotation_matrix)
{
	
	float tr, s, q[4];
	int i, j, k;
	int nxt[3] = { 1, 2, 0 };
	tr = rotation_matrix[0][0] + rotation_matrix[1][1] + rotation_matrix[2][2];

	if (tr > 0)
	{
		s = sqrt(tr + 1.0f);
		this->m_w = s / 2.0f;
		s = 0.5f / s;
		this->m_x = (rotation_matrix[1][2] - rotation_matrix[2][1])*s;
		this->m_y = (rotation_matrix[2][0] - rotation_matrix[0][2])*s;
		this->m_z = (rotation_matrix[0][1] - rotation_matrix[1][0])*s;
	}
	else
	{
		i = 0;

		if (rotation_matrix[1][1] > rotation_matrix[0][0])
		{
			i = 1;
		}

		if (rotation_matrix[2][2] > rotation_matrix[i][i])
		{
			i = 2;
		}

		j = nxt[i];
		k = nxt[j];
		s = sqrt((rotation_matrix[i][i] - (rotation_matrix[j][j] + rotation_matrix[k][k])) + 1.0);
		q[i] = s *0.5;

		if (s != 0.0)
		{ 
			s = 0.5 / s; 
		}

		q[3] = (rotation_matrix[j][k] - rotation_matrix[k][j])*s;
		q[j] = (rotation_matrix[i][j] + rotation_matrix[j][i])*s;
		q[k] = (rotation_matrix[i][k] + rotation_matrix[k][i])*s;

		this->m_x = q[0];
		this->m_y = q[1];
		this->m_z = q[2];
		this->m_w = q[3];
	}
}

void Quaternion::EulerToQuaternion( float roll, float pitch, float yaw )
{

	float cos_roll, cos_pitch, cos_yaw, sin_roll, sin_pitch, sin_yaw, cos_p_cos_y, sin_p_sin_y;

	cos_roll = cosf(roll / 2);
	cos_pitch = cosf(pitch / 2);
	cos_yaw = cosf(yaw / 2);

	sin_roll = sinf(roll / 2);
	sin_pitch = sinf(pitch / 2);
	sin_yaw = sinf(yaw / 2);

	cos_p_cos_y = cos_pitch * cos_yaw;
	sin_p_sin_y = sin_pitch * sin_yaw;
	
	this->m_w = cos_roll * cos_p_cos_y + sin_roll * sin_p_sin_y;
	this->m_x = sin_roll * cos_p_cos_y - cos_roll * sin_p_sin_y;
	this->m_y = cos_roll * sin_pitch * cos_yaw + sin_roll * cos_pitch * sin_yaw;
	this->m_z = cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw;
}

void Quaternion::SlerpQuaternion(Quaternion &result, Quaternion *target, float t)
{
	float           target1[4];
	double			omega, cosom, sinom, scale0, scale1;

	Quaternion* from = this;

	cosom = from->m_x * target->m_x + from->m_y * target->m_y + from->m_z * target->m_z
		+ from->m_w * target->m_w;

	if (cosom < 0.0f)
	{
		cosom = -cosom; target1[0] = -target->m_x;
		target1[1] = -target->m_y;
		target1[2] = -target->m_z;
		target1[3] = -target->m_w;
	}
	else  
	{
		target1[0] = target->m_x;
		target1[1] = target->m_y;
		target1[2] = target->m_z;
		target1[3] = target->m_w;
	}

	if ((1.0f - cosom) > DELTA) 
	{
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1.0f - t) * omega) / sinom;
		scale1 = sin(t * omega) / sinom;
	}
	else 
	{
		scale0 = 1.0f - t;
		scale1 = t;
	}

	result.m_x = scale0 * from->m_x + scale1 * target1[0];
	result.m_y = scale0 * from->m_y + scale1 * target1[1];
	result.m_z = scale0 * from->m_z + scale1 * target1[2];
	result.m_w = scale0 * from->m_w + scale1 * target1[3];
}

Quaternion::~Quaternion()
{

}
