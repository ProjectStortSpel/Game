#include "VRAMInfo.h"
#include "Renderer\stdafx.h"

VRAMInfo::VRAMInfo()
{
}

VRAMInfo::~VRAMInfo()
{
}

int VRAMInfo::GetFreeVideoMemory()
{
	int availableKB[4];
	availableKB[0] = -1;
	int totalAvailable = 0;

	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &availableKB[0]);
	int tmp[4];
	if (availableKB[0] < 0)
	{
		glGetIntegerv(GL_VBO_FREE_MEMORY_ATI, availableKB);
		totalAvailable += availableKB[0];


		glGetError();

		return totalAvailable;
	}

}