#ifndef STDAFX_NETWORK_H
#define STDAFX_NETWORK_H

#include <SDL/SDL.h>
#include <string>

#include "Packet.h"

// Includes
#ifdef WIN32
#else
#include <unistd.h>
#endif

// Macros/Defines
#ifdef WIN32
#define NetSleep(x) Sleep(x);
#else
#define NetSleep(x) usleep(30 * 1000);
#endif

#define MAX_PACKET_SIZE 65535 // Max value for unsigned short
#define NET_DEBUG 1
#define SAFE_DELETE(x) if(x) { delete x; x = 0; }
#define SAFE_DELETE_PACKET(x) if(x) { if (x->Data) { delete x->Data; x->Data = 0; } delete x; x = 0; }

#endif