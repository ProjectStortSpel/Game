#include <SDL/SDL.h>
#include "Network/Server.h"
#include "Network/Client.h"


Client *c = new Client();
bool connected = false;

void OnConnect(unsigned char _token)
{
	connected = true;
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

	packet.StartPack("hest");
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
	auto c1 = packet.ReadByte();			printf("Server unpacking: '%c'\n", c1);
	auto f1 = packet.ReadFloat();			printf("Server unpacking: %f\n", f1);
	auto i1 = packet.ReadInt();				printf("Server unpacking: %i\n", i1);
	auto s1 = packet.ReadString();			printf("Server unpacking: \"%s\"\n", s1);
	packet.EndUnPack();

	//delete packetIn.Data;

	packet.StartPack("test2");
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
	auto c2 = packet.ReadByte();			printf("Client unpacking: '%c'\n", c2);
	auto f2 = packet.ReadFloat();			printf("Client unpacking: %f\n", f2);
	auto i2 = packet.ReadInt();				printf("Client unpacking: %i\n", i2);
	auto s2 = packet.ReadString();			printf("Client unpacking: \"%s\"\n", s2);

	float f3 = 0.2f;

	c->Disconect();
	s->Stop();

	system("pause");

	SDL_Quit();
	return 0;
}