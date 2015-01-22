#ifndef QUATERNION_H
#define QUATERNION_H

#include <glm/glm.hpp>

class Quaternion
{
public:
	Quaternion operator* (Quaternion quaternion);

	Quaternion();
	/*
		Rotate()
		Parameter 
			vector	: is the values of the vector which the object shoud rotate around
			w		: the amount of rotation 
	*/
	void Rotate(const glm::vec3 &vector, float w);
	/*
		normalize the quaternion
	*/
	void Normalize();
	/*
		Return the rotation matrix
	*/
	glm::mat4 QuaternionToMatrix();

	/*
		MatrixToQuaternion()
		Parameter
			rotation_matrix : stor this matrix as a Quaternion
	*/
	void MatrixToQuaternion( const glm::mat4 &rotation_matrix);
	/*
		EulerToQuaternion()
		convert from euler to quaternion standards
	*/
	void EulerToQuaternion( float roll, float pich, float yaw );
	/*
		SlerpQuaternion()
		slerp from this to target quaternion
	*/
	void SlerpQuaternion(Quaternion &result, Quaternion *target, float t);

	~Quaternion();

	float m_w;
	float m_x;
	float m_y;
	float m_z;

};

#endif