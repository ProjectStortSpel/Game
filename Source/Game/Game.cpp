#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <ECSL/ECSL.h>
#include <vector>
#include <algorithm>
#include <time.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace ECSL;

class TestSystem : public ECSL::System
{
public:
	TestSystem() { }
	~TestSystem() { }

	void Run()
	{
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

std::map<int, bool>* testMap;
ECSL::BitSet::DataType* testSet;
SDL_mutex* mutex;
void PerfTest(void* data)
{
	unsigned int entityCount = 10000;
	unsigned int numInSystem = 3000;

	testSet = ECSL::BitSet::GenerateBitSet(entityCount);
	testMap = new std::map<int, bool>();
	std::vector<unsigned int> loop;

	srand((unsigned int)time(NULL));

	for (unsigned int i = 0; i < numInSystem; ++i)
	{
		unsigned int number = rand() % entityCount;
		if (testMap->count(number) == 1)
		{
			--i;
			continue;
		}
		unsigned int bitIndex = ECSL::BitSet::GetBitIndex(number);
		unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(number);
		testSet[bitSetIndex] |= ((ECSL::BitSet::DataType)1) << bitIndex;
		(*testMap)[number] = true;
		loop.push_back(number);
	}
	std::random_shuffle(loop.begin(), loop.end());

	float testVal1 = 0;
	float testVal2 = 0;

	clock_t start1, start2, end1, end2;
	start1 = clock();
	for (unsigned int i = 0; i < 1000; ++i)
	{
		for (unsigned int n = 0; n < numInSystem; ++n)
		{
			unsigned int number = loop[n];
			unsigned int bitIndex = ECSL::BitSet::GetBitIndex(number);
			unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(number);
			if (testSet[bitSetIndex] & ((ECSL::BitSet::DataType)1) << bitIndex)
				testVal1 += n;
		}
	}
	end1 = clock();

	start2 = clock();
	for (unsigned int i = 0; i < 1000; ++i)
	{
		
		for (unsigned int n = 0; n < numInSystem; ++n)
		{
			SDL_LockMutex(mutex);
			unsigned int number = loop[n];
			unsigned int bitIndex = ECSL::BitSet::GetBitIndex(number);
			unsigned int bitSetIndex = ECSL::BitSet::GetBitSetIndex(number);
			if (testSet[bitSetIndex] & ((ECSL::BitSet::DataType)1) << bitIndex)
				testVal2 += n;
			SDL_UnlockMutex(mutex);
		}
		

		//for (unsigned int n = 0; n < numInSystem; ++n)
		//{
		//	unsigned int number = loop[n];
		//	auto it = testMap->find(number);
		//	if (it != testMap->end())
		//		testVal2 += n;
		//}
	}
	end2 = clock();

	printf("Test1 Time: %f\n", (float)(end1 - start1) / CLOCKS_PER_SEC);
	printf("Test2 Time: %f\n", (float)(end2 - start2) / CLOCKS_PER_SEC);
	printf("%f, %f\n", testVal1, testVal2);

	delete testMap;
	delete testSet;
}

void lol()
{
	ComponentTypeManager::GetInstance().LoadComponentTypesFromDirectory("Content/components");
	ECSL::WorldCreator worldCreator = ECSL::WorldCreator();
	worldCreator.AddSystemGroup();
	worldCreator.AddSystemToCurrentGroup<TestSystem>();
	worldCreator.AddComponentType("Position");
	worldCreator.AddComponentType("Velocity");
	ECSL::World* world = worldCreator.CreateWorld(100);

	int id = world->CreateNewEntity();
	world->CreateComponentAndAddTo("Velocity", id);
	world->CreateComponentAndAddTo("Position", id);
	world->RemoveComponentFrom("Position", id);
	world->CreateComponentAndAddTo("Position", id);

	world->KillEntity(id);

	delete(world);
	delete(&ComponentTypeManager::GetInstance());
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	lol();
	SDL_Thread* thread1;
	mutex = SDL_CreateMutex();
	//if (!mutex)
	//	printf("Mutex...");
	//thread1 = SDL_CreateThread((SDL_ThreadFunction)PerfTest, "PerfTest", (void*)0);
	//SDL_WaitThread(thread1, 0);
	PerfTest(0);
	SDL_DestroyMutex(mutex);
	SDL_Quit();
	system("pause");
	_CrtDumpMemoryLeaks();
	return 0;
}