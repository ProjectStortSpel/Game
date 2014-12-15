#include "Network/Stdafx.h"

unsigned long long Network::htonll(unsigned long long src)
{
	static int typ = TYP_INIT;
	unsigned char c;
	union 
	{
		unsigned long long ull;
		unsigned char c[8];
	} x;
	if (typ == TYP_INIT)
	{
		x.ull = 0x01;
		typ = (x.c[7] == 0x01ULL) ? TYP_BIGE : TYP_SMLE;
	}
	if (typ == TYP_BIGE)
		return src;
	x.ull = src;
	c = x.c[0]; x.c[0] = x.c[7]; x.c[7] = c;
	c = x.c[1]; x.c[1] = x.c[6]; x.c[6] = c;
	c = x.c[2]; x.c[2] = x.c[5]; x.c[5] = c;
	c = x.c[3]; x.c[3] = x.c[4]; x.c[4] = c;
	return x.ull;
}

unsigned long long Network::ntohll(unsigned long long src)
{
	return Network::htonll(src);
}