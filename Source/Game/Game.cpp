#include <SDL/SDL.h>
#include "Network/Server.h"
#include "Network/Client.h"


Client *c = new Client();
bool connected = false;

void OnConnect(unsigned char _token)
{
	connected = true;
}

void TestHook(PacketHandler* _ph)
{
	auto c1 = _ph->ReadByte();			printf("Server unpacking: '%c'\n", c1);
	auto f1 = _ph->ReadFloat();			printf("Server unpacking: %f\n", f1);
	auto i1 = _ph->ReadInt();				printf("Server unpacking: %i\n", i1);
	auto s1 = _ph->ReadString();			printf("Server unpacking: \"%s\"\n", s1);
}

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);


	Server *s = new Server();
	s->Start();

	c->SetOnConnectedToServer(&OnConnect);
	c->Connect();
	c->StartListen();

	while (!connected)
		NetSleep(30);

	PacketHandler packet;

	packet.AddNetMessageHook("TestHook", &TestHook);

	packet.StartPack("TestHook");
	packet.WriteByte('p');					printf("Client packing: 'p'\n");
	packet.WriteFloat(0.5f);				printf("Client packing: 0.5\n");
	packet.WriteInt(1337);					printf("Client packing: 1337\n");
	packet.WriteString("HEST HEST HEST2");	printf("Client packing: \"HEST HEST HEST2\"\n");
	auto p = packet.EndPack();
	//system("pause");
	c->SendToServer(p);						printf("Client sending packet\n\n");


	PacketHandler::Packet* packetIn = 0;
	while (!packetIn)
		packetIn = s->GetPacket();

	packet.StartUnPack(packetIn);			printf("Server reciveing packet\n");

	//delete packetIn.Data;

	packet.StartPack("TestHooker");
	packet.WriteByte('p');					printf("Server packing: 'p'\n");
	packet.WriteFloat(0.2f);				printf("Server packing: 0.2\n");
	packet.WriteInt(107);					printf("Server packing: 107\n");
	packet.WriteString("Testar en sak");	printf("Server packing: \"Testar en sak\"\n");
	p = packet.EndPack();

	s->Broadcast(p);						printf("Server sending packet\n\n");

	packetIn = 0;
	while (!packetIn)
		packetIn = c->GetPacket();

	packet.StartUnPack(packetIn);			printf("Client reciveing packet\n");

	float f3 = 0.2f;

	c->Disconect();
	s->Stop();

	system("pause");

	SDL_Quit();
	return 0;
}