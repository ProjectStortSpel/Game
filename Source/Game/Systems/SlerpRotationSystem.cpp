#include "SlerpRotationSystem.h"
#include "Game/NetworkInstance.h"

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
		if (!NetworkInstance::GetServer()->IsRunning())
		{
			float* from_data = (float*)GetComponent(entities[i], "SlerpRotation", "fromW");
			float* to_data = (float*)GetComponent(entities[i], "SlerpRotation", "toW");

			Quaternion q_from, q_to;

			q_from.Rotate(glm::vec3(0, 1, 0), *from_data);
			q_to.Rotate(glm::vec3(0, 1, 0), *to_data);
		
			q_to = q_from*q_to;

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
}

void SlerpRotationSystem::OnEntityAdded(unsigned int _entityId)
{
	//printf("Adding\n");
	unsigned int entity = _entityId;
	
	float* a = (float*)GetComponent(entity, "SlerpRotation", "time");
	
	(*a) = 0.0;
}

void SlerpRotationSystem::OnEntityRemoved(unsigned int _entityId)
{

}

SlerpRotationSystem::~SlerpRotationSystem()
{
}
