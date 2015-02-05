#include "SlerpRotationSystem.h"
#include "Game/Network/NetworkInstance.h"

SlerpRotationSystem::SlerpRotationSystem()
{
}

void SlerpRotationSystem::Initialize()
{
	SetSystemName("Slerp Rotation System");

	SetUpdateTaskCount(1);
	SetEntitiesAddedTaskCount(1);

	AddComponentTypeToFilter("SlerpRotation", ECSL::FilterType::Mandatory);
	AddComponentTypeToFilter("Rotation", ECSL::FilterType::Mandatory);

	m_rotationId = ECSL::ComponentTypeManager::GetInstance().GetTableId("Rotation");

	printf("Slerp Rotation System Initialized!\n");
}

void SlerpRotationSystem::Update(const ECSL::RuntimeInfo& _runtime)
{
	
	if (NetworkInstance::GetServer()->IsRunning())
	{
		std::vector<unsigned int> entities = *GetEntities();
		float _dt = _runtime.Dt; 
		unsigned int startAt, endAt;
		MPL::MathHelper::SplitIterations(startAt, endAt, (unsigned int)entities.size(), _runtime.TaskIndex, _runtime.TaskCount);
		for (int i = startAt; i < endAt; i++)
		{
			printf("entities %i\n", i);
			float* time_data = (float*)GetComponent(entities[i], "SlerpRotation", "time");
			float* from_data = (float*)GetComponent(entities[i], "SlerpRotation", "fromW");
			float* from_dataX = (float*)GetComponent(entities[i], "SlerpRotation", "fromX");
			float* from_dataY = (float*)GetComponent(entities[i], "SlerpRotation", "fromY");
			float* from_dataZ = (float*)GetComponent(entities[i], "SlerpRotation", "fromZ");
			float* to_data = (float*)GetComponent(entities[i], "SlerpRotation", "toW");
			float* to_dataX = (float*)GetComponent(entities[i], "SlerpRotation", "toX");
			float* to_dataY = (float*)GetComponent(entities[i], "SlerpRotation", "toY");
			float* to_dataZ = (float*)GetComponent(entities[i], "SlerpRotation", "toZ");

			Quaternion q_from, q_to;
			Quaternion current_rotation;

			q_from.m_w = *from_data;
			q_from.m_x = *from_dataX;
			q_from.m_y = *from_dataY;
			q_from.m_z = *from_dataZ;

			q_to.m_w = *to_data;
			q_to.m_x = *to_dataX;
			q_to.m_y = *to_dataY;
			q_to.m_z = *to_dataZ;

			glm::vec3 euler;

			(*time_data) += _dt * 1.2f;

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

void SlerpRotationSystem::EntitiesAdded(const ECSL::RuntimeInfo& _runtime, const std::vector<unsigned int>& _entities)
{
	for (int i = 0; i < _entities.size(); ++i)
	{
		unsigned int _entityId = _entities[i];
		//printf("Adding\n");
		unsigned int entity = _entityId;

		float* time_data = (float*)GetComponent(_entityId, "SlerpRotation", "time");
		float* from_data = (float*)GetComponent(_entityId, "SlerpRotation", "fromW");
		float* from_dataX = (float*)GetComponent(_entityId, "SlerpRotation", "fromX");
		float* from_dataY = (float*)GetComponent(_entityId, "SlerpRotation", "fromY");
		float* from_dataZ = (float*)GetComponent(_entityId, "SlerpRotation", "fromZ");
		float* to_data = (float*)GetComponent(_entityId, "SlerpRotation", "toW");
		float* to_dataX = (float*)GetComponent(_entityId, "SlerpRotation", "toX");
		float* to_dataY = (float*)GetComponent(_entityId, "SlerpRotation", "toY");
		float* to_dataZ = (float*)GetComponent(_entityId, "SlerpRotation", "toZ");

		Quaternion q_from, q_to;

		q_to.Rotate(glm::vec3(*to_dataX, *to_dataY, *to_dataZ), *to_data);

		q_from.Rotate(glm::vec3(*from_dataX, *from_dataY, *from_dataZ), *from_data);

		q_to = q_from*q_to;

		*time_data = 0.0;
		*from_data	= q_from.m_w;
		*from_dataX	= q_from.m_x;
		*from_dataY	= q_from.m_y;
		*from_dataZ	= q_from.m_z;
		*to_data 	= q_to.m_w;
		*to_dataX	= q_to.m_x;
		*to_dataY	= q_to.m_y;
		*to_dataZ	= q_to.m_z;
	}
}


SlerpRotationSystem::~SlerpRotationSystem()
{
}
