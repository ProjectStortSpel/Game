#include "SlerpRotationSystem.h"


SlerpRotationSystem::SlerpRotationSystem()
{
}

void SlerpRotationSystem::Initialize()
{
	SetSystemName("Slerp Rotation System");

	AddComponentTypeToFilter("SlerpRotation", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation", ECSL::FilterType::Mandatory);

	m_rotationId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation");

	printf("Slerp Rotation System Initialized!\n");
}

void SlerpRotationSystem::Update(float _dt)
{
	std::vector<unsigned int> entities = *GetEntities();

	for (int i = 0; i < entities.size(); i++)
	{
		float* from_data = (float*)GetComponent(entities[i], "SlerpRotation", "fromX");
		float* to_data = (float*)GetComponent(entities[i], "SlerpRotation", "toX");

		Quaternion q_from, q_to;

		q_from.m_x = from_data[0];
		q_from.m_y = from_data[1];
		q_from.m_z = from_data[2];
		q_from.m_w = from_data[3];

		q_to.m_x = to_data[0];
		q_to.m_y = to_data[1];
		q_to.m_z = to_data[2];
		q_to.m_w = to_data[3];

		//q_from.Rotate(glm::vec3(0, 1, 0), 3.14);
		//q_to.Rotate(glm::vec3(0, 1, 0), 3.14);
		//
		//q_to = q_from*q_to;

		//glm::vec3 data( to_data[0], to_data[1], to_data[2] );

		//q_to.Rotate(data, to_data[3]);

		//printf("%f,%f,%f,%f\n", from_data[0], from_data[1], from_data[2], from_data[3]);
		//printf("%f,%f,%f,%f\n", to_data[0], to_data[1], to_data[2], to_data[3]);

		Quaternion current_rotation;

		float* time_data = (float*)GetComponent(entities[i], "SlerpRotation", "time");

		glm::vec3 euler;

		(*time_data) += _dt; 

		if ((*time_data) > 1)
		{
			(*time_data) = 0;
			RemoveComponentFrom("SlerpRotation", entities[i]);
			euler = q_to.QuaternionToEuler();
		}
		else
		{
			q_from.SlerpQuaternion(current_rotation, &q_to, (*time_data));
			euler = current_rotation.QuaternionToEuler();
		}


		float* temp = (float*)GetComponent(entities[i], "Rotation", "X");

		temp[0] = euler.x;
		temp[1] = euler.y;
		temp[2] = euler.z;

		ComponentHasChanged(entities[i], m_rotationId);
	}
}

void SlerpRotationSystem::OnEntityAdded(unsigned int _entityId)
{
	printf("Adding\n");
	unsigned int entity = _entityId;
	
	float* from_euler_data = (float*)GetComponent(entity, "SlerpRotation", "fromX");
	float* to_euler_data = (float*)GetComponent(entity, "SlerpRotation", "toX");
	/*
	printf("Euler data\n");
	printf("%f,%f,%f,%f\n", from_euler_data[0], from_euler_data[1], from_euler_data[2], from_euler_data[3]);
	printf("%f,%f,%f,%f\n", to_euler_data[0], to_euler_data[1], to_euler_data[2], to_euler_data[3]);
	*/

	Quaternion q_from;
	Quaternion q_to;
	
	//q_to.Rotate(glm::vec3(to_euler_data[0], to_euler_data[1], to_euler_data[2]), to_euler_data[4]);
	
	//q_to.m_x = to_euler_data[0];
	//q_to.m_y = to_euler_data[1];
	//q_to.m_z = to_euler_data[2];
	//q_to.m_w = to_euler_data[3];

	//q_from.m_x = from_euler_data[0];
	//q_from.m_y = from_euler_data[1];
	//q_from.m_z = from_euler_data[2];
	//q_from.m_w = from_euler_data[3];

	//q_from.Rotate(glm::vec3(from_euler_data[0], from_euler_data[1], from_euler_data[2]), from_euler_data[3]);
	//q_to.Rotate(glm::vec3(to_euler_data[0], to_euler_data[1], to_euler_data[2]), to_euler_data[3]);
	
	q_from.Rotate(glm::vec3(0,1,0), 0);
	q_to.Rotate(glm::vec3(0,1,0), 3.14);

	q_to = q_from * q_to;
	
	float from_data[4];
	from_data[0] = q_from.m_x;
	from_data[1] = q_from.m_y;
	from_data[2] = q_from.m_x;
	from_data[3] = q_from.m_w;
	
	float to_data[4];
	to_data[0] = q_to.m_x;
	to_data[1] = q_to.m_y;
	to_data[2] = q_to.m_x;
	to_data[3] = q_to.m_w;

	//float from_data[4];
	//from_data[0] = 0;
	//from_data[1] = 1;
	//from_data[2] = 0;
	//from_data[3] = 0;
	//
	//float to_data[4];
	//to_data[0] = 0;
	//to_data[1] = 1;
	//to_data[2] = 0;
	//to_data[3] = 3.14;

	//printf("Quaternion data\n");
	//printf("%f,%f,%f,%f\n", from_data[0], from_data[1], from_data[2], from_data[3]);
	//printf("%f,%f,%f,%f\n", to_data[0], to_data[1], to_data[2], to_data[3]);

	from_euler_data[0] = from_data[0];
	from_euler_data[1] = from_data[1];
	from_euler_data[2] = from_data[2];
	from_euler_data[3] = from_data[3];
	
	to_euler_data[0] = to_data[0];
	to_euler_data[1] = to_data[1];
	to_euler_data[2] = to_data[2];
	to_euler_data[3] = to_data[3];

	printf("Entity Added!\n");
	printf("from : %f,%f,%f,%f\n", from_data[0], from_data[1], from_data[2], from_data[3]);
	printf("to   : %f,%f,%f,%f\n", to_data[0], to_data[1], to_data[2], to_data[3]);
	
	float* a = (float*)GetComponent(entity, "SlerpRotation", "time");
	
	*a = 0.0;

	//printf("Entity Added!\n");
	//printf("from : %f,%f,%f,%f\n", from_euler_data[0], from_euler_data[1], from_euler_data[2], from_euler_data[3]);
	//printf("to   : %f,%f,%f,%f\n", to_euler_data[0], to_euler_data[1], to_euler_data[2], to_euler_data[3]);
}

void SlerpRotationSystem::OnEntityRemoved(unsigned int _entityId)
{

}

SlerpRotationSystem::~SlerpRotationSystem()
{
}
