#include <SDL/SDL.h>
#include "Network/PacketHandler.h"
#include "Network/Client.h"
#include "Network/Server.h"


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
	packet.WriteByte('p');
	packet.WriteFloat(0.5f);
	packet.WriteInt(1337);
	packet.WriteString("HEST HEST HEST2");
	auto p = packet.EndPack();
	//system("pause");
	c->SendToServer(p);


	PacketHandler::Packet* packetIn = 0;
	while(!packetIn)
		packetIn = s->GetPacket();

	packet.StartUnPack(packetIn);
		auto c1 = packet.ReadByte();
		auto f1 = packet.ReadFloat();
		auto i1 = packet.ReadInt();
		auto s1 = packet.ReadString();
	packet.EndUnPack();

	//delete packetIn.Data;

	packet.StartPack("test2");
	packet.WriteByte('p');
	packet.WriteFloat(0.2f);
	packet.WriteInt(107);
	packet.WriteString("Testar en sak");
	p = packet.EndPack();
	
	s->Broadcast(p);

	packetIn = 0;
	while (!packetIn)
		packetIn = c->GetPacket();

	packet.StartUnPack(packetIn);
	auto c2 = packet.ReadByte();
	auto f2 = packet.ReadFloat();
	auto i2 = packet.ReadInt();
	auto s2 = packet.ReadString();

	float f3 = 0.2f;

	c->Disconect();
	s->Stop();

	system("pause");

	SDL_Quit();
	return 0;
}