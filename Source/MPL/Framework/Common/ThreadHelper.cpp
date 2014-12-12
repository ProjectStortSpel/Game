#include "ThreadHelper.h"

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#endif

using namespace MPL;

int ThreadHelper::GetCurrentThreadCount()
{
	#if defined(_WIN32)
	// first determine the id of the current process
	DWORD const  id = GetCurrentProcessId();

	// then get a process list snapshot.
	HANDLE const  snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	// initialize the process entry structure.
	PROCESSENTRY32 entry = { 0 };
	entry.dwSize = sizeof(entry);

	// get the first process info.
	BOOL  ret = true;
	ret = Process32First(snapshot, &entry);
	while (ret && entry.th32ProcessID != id) {
		ret = Process32Next(snapshot, &entry);
	}
	CloseHandle(snapshot);
	return ret
		? entry.cntThreads
		: -1;

	#else

	return SDL_GetCPUCount();

	#endif
}

unsigned int ThreadHelper::GetAvailableThreadCount()
{
	/* Fetch number of currently used threads */
	int currentThreadCount = ThreadHelper::GetCurrentThreadCount();
	if (currentThreadCount == -1)
	{
		currentThreadCount = SDL_GetCPUCount();
	}

	/* Total number of logical processors - Number of threads currently used = Number of available threads */
	return (unsigned int)(SDL_GetCPUCount() - currentThreadCount);
}