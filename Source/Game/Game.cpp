#include <SDL/SDL.h>
#include "Network/PacketHandler.h"
#include "Network/Client.h"
#include "Network/Server.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	Client *c = new Client();
	Server *s = new Server();

	s->SetNetPort(1234);
	s->Connect();

	c->SetIp("localhost");
	c->SetNetPort(1234);
	c->SetClientPort(12345);
	c->Connect();

	PacketHandler packet;

	packet.StartPack("hest");
	packet.WriteByte('p');
	packet.WriteFloat(0.5f);
	packet.WriteInt(1337);
	packet.WriteString("HEST HEST HEST2");
	auto p = packet.EndPack();
	system("pause");
	c->Send(p);


	PacketHandler::Packet* packetIn = NULL;
	while (!packetIn)
	{
		packetIn = s->GetPacket();
	}

	packet.StartUnPack(*packetIn);
	auto c1 = packet.ReadByte();
	auto f1 = packet.ReadFloat();
	auto i1 = packet.ReadInt();
	auto s1 = packet.ReadString();

	packet.StartPack("test2");
	packet.WriteByte('p');
	packet.WriteFloat(0.2f);
	packet.WriteInt(107);
	packet.WriteString("Testar en sak");
	p = packet.EndPack();
	
	s->Broadcast(p);


	packetIn = NULL;
	while (!packetIn)
	{
		packetIn = c->GetPacket();
	}

	packet.StartUnPack(*packetIn);
	auto c2 = packet.ReadByte();
	auto f2 = packet.ReadFloat();
	auto i2 = packet.ReadInt();
	auto s2 = packet.ReadString();

	c->Disconect();
	s->Disconect();

	system("pause");

	SDL_Quit();
	return 0;
}