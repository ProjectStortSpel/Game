#ifndef VRAMINFO_H
#define VRAMINFO_H


class VRAMInfo
{
public:
	VRAMInfo();
	~VRAMInfo();

	static int GetFreeVideoMemory();
};

#endif