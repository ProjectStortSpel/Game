#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <ECSL/ECSL.h>
#include <vector>
#include <algorithm>
#include <time.h>
#ifdef WIN32
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

using namespace ECSL;

class TestSystem : public ECSL::System
{
public:
	TestSystem() { }
	~TestSystem() { }

	void Update(float _dt)
	{
		const std::vector<unsigned int> entities = *GetEntities();
		for (auto entity : entities)
		{
			float* x = (float*)GetComponent(entity, "Position", "X");
			*x = *x + 5;
			printf("Position: %f, %f\n", *(float*)GetComponent(entity, "Position", "X"), *(float*)GetComponent(entity, "Position", "Y"));
		}
		printf("Testsystem run()\n");
	}
	void Initialize()
	{
		AddComponentTypeToFilter("Position", ECSL::FilterType::Mandatory);
		//AddComponentTypeToFilter("Velocity", ECSL::FilterType::RequiresOneOf);
		//AddComponentTypeToFilter("Velocity", ECSL::ComponentFilter::RequiresOneOf);
		//AddComponentTypeToFilter("Position", ECSL::ComponentFilter::Excluded);

		printf("Testsystem Initialize()\n");
	}

	void OnEntityAdded(unsigned int _entityId)
	{
		printf("Testsystem OnEntityAdded()\n");
	}
	void OnEntityRemoved(unsigned int _entityId)
	{
		printf("Testsystem OnEntityRemoved()\n");
	}
private:
};



std::vector<unsigned int>* testVec;
ECSL::BitSet::DataType* testSet;
SDL_mutex* mutex;
void PerfTest(void* data)
{
	unsigned int entityCount = 10000;
	unsigned int entitiesInSystem = 1500;

	testSet = BitSet::GenerateBitSet(entityCount);
	unsigned int biggestEntityNum = 0;
	testVec = new std::vector<unsigned int>();

	srand(time(NULL));

	for (int i = 0; i < entitiesInSystem; ++i)
	{
		int val = rand() % entityCount;
		unsigned int bitIndex = BitSet::GetBitIndex(val);
		unsigned int bitSetIndex = BitSet::GetBitSetIndex(val);
		if (testSet[bitSetIndex] & ((BitSet::DataType)1) << bitIndex)
		{
			i--;
			continue;
		}
		testSet[bitSetIndex] |= ((BitSet::DataType)1) << bitIndex;
		if (val > biggestEntityNum)
			biggestEntityNum = val;
		testVec->push_back(val);
	}


	float testVal1 = 0;
	float testVal2 = 0;
	float abc = 1.0f;
	float* abc2 = new float();
	*abc2 = 1.0f;

	clock_t start1, start2, end1, end2;
	start1 = clock();
	for (unsigned int i = 0; i < 1000000000; ++i)
	{
		testVal1 += abc;
	}
	end1 = clock();

	start2 = clock();
	for (unsigned int i = 0; i < 1000000000; ++i)
	{
		testVal2 += *abc2;
	}
	end2 = clock();

	printf("Test1 Time: %f\n", (float)(end1 - start1) / CLOCKS_PER_SEC);
	printf("Test2 Time: %f\n", (float)(end2 - start2) / CLOCKS_PER_SEC);
	printf("%f, %f\n", testVal1, testVal2);

	delete testVec;
	delete testSet;
}

DataArray* ar1;
DataArray* ar2;

void lol()
{
	ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("Content/components");
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	worldCreator.AddSystemGroup();
	worldCreator.AddSystemToCurrentGroup<TestSystem>();
	auto componentTypes = ComponentTypeManager::GetInstance().GetComponentTypes();
	for (auto it = componentTypes->begin(); it != componentTypes->end(); ++it)
		worldCreator.AddComponentType(it->second->GetName());
	ECSL::World* world = worldCreator.CreateWorld(10000);

	int id = world->CreateNewEntity();
	//world->CreateComponentAndAddTo("Velocity", id);
	//float x = 4;
	//world->SetComponent("Velocity", "X", &x, id);
	
	world->CreateComponentAndAddTo("Position", id);
	//world->RemoveComponentFrom("Position", id);
	//world->CreateComponentAndAddTo("Position", id);

	//world->KillEntity(id);

	world->Update(0.01f);

	//world->KillEntity(id);
	//world->CreateComponentAndAddTo("Position", id);

	world->Update(0.01f);

	//world->RemoveComponentFrom("Position", id);

	world->Update(0.01f);

	delete(world);
	delete(&ComponentTypeManager::GetInstance());
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	lol();
	mutex = SDL_CreateMutex();
	//if (!mutex)
	//	printf("Mutex...");
	//thread1 = SDL_CreateThread((SDL_ThreadFunction)PerfTest, "PerfTest", (void*)0);
	//SDL_WaitThread(thread1, 0);
	//PerfTest(0);
	SDL_DestroyMutex(mutex);
	SDL_Quit();
	system("pause");
	#ifdef WIN32
	_CrtDumpMemoryLeaks();
	#endif
	return 0;
}