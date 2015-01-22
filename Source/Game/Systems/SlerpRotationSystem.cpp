#include "SlerpRotationSystem.h"


SlerpRotationSystem::SlerpRotationSystem()
{
}

void SlerpRotationSystem::Initialize()
{
	SetSystemName("Slerp Rotation System");
	
	AddComponentTypeToFilter("SlerpRotation", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation", ECSL::FilterType::Mandatory);

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
		
		printf("%f,%f,%f,%f\n", from_data[0], from_data[1], from_data[2], from_data[3]);
		printf("%f,%f,%f,%f\n", to_data[0], to_data[1], to_data[2], to_data[3]);

		Quaternion current_rotation;

		float* time_data = (float*)GetComponent(entities[i], "SlerpRotation", "time"); 

		*time_data += _dt;
		if (*time_data > 1)
		{
			*time_data = 1;
			RemoveComponentFrom("SlerpRotation", entities[i]);
		}

		q_from.SlerpQuaternion(current_rotation, &q_to, *time_data);

		glm::vec3 euler = current_rotation.QuaternionToEuler();

		float data[3];

		data[0] = euler.x;
		data[1] = euler.y;
		data[2] = euler.z;
		
		float* fuck_u_all = (float*)GetComponent(entities[i], "Rotation", "X");

		fuck_u_all[0] = euler.x;
		fuck_u_all[1] = euler.y;
		fuck_u_all[2] = euler.z;

	}
}

void SlerpRotationSystem::OnEntityAdded(unsigned int _entityId)
{
	unsigned int entity = _entityId;

	float* from_euler_data	= (float*)GetComponent(entity, "SlerpRotation", "fromX");
	float* to_euler_data	= (float*)GetComponent(entity, "SlerpRotation", "toX");

	Quaternion q_from, q_to;

	q_from.EulerToQuaternion( from_euler_data[0], from_euler_data[1], from_euler_data[2] );
	q_to.EulerToQuaternion( to_euler_data[0], to_euler_data[1], to_euler_data[2] );
	q_from.Normalize();
	q_to.Normalize();

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

	printf("%f,%f,%f,%f\n", from_data[0], from_data[1], from_data[2], from_data[3]);
	printf("%f,%f,%f,%f\n", to_data[0], to_data[1], to_data[2], to_data[3]);

	float time = 0;

	from_euler_data[0] = from_data[0];
	from_euler_data[1] = from_data[1];
	from_euler_data[2] = from_data[2];
	from_euler_data[3] = from_data[3];

	to_euler_data[0] = to_data[0];
	to_euler_data[1] = to_data[1];
	to_euler_data[2] = to_data[2];
	to_euler_data[3] = to_data[3];

	//SetComponent(entity, "SlerpRotation", "fromX", from_data);
	//SetComponent(entity, "SlerpRotation", "toX", to_data);
	SetComponent(entity, "SlerpRotation", "time", &time);
}

void SlerpRotationSystem::OnEntityRemoved(unsigned int _entityId)
{

}

SlerpRotationSystem::~SlerpRotationSystem()
{
}
