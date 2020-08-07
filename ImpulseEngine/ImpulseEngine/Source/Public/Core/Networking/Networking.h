#pragma once

struct _ENetHost;
struct _ENetPeer;

namespace httplib {
	class Client;
}

class Portfwd;

namespace GEngine {

	class Networking {
	public:
		static void Init();
		static void Shutdown();
		static void Reset();

		static void CreateServer(uint16_t port = 27015, uint16_t listenRate = 1000, unsigned char maxClient = 32, unsigned char channels = 2, uint32_t incomingBandwith = 0, uint32_t outgoingBandwidth = 0);
		static void CreateClient();

		static void ConnectToServer(std::string ip = "127.0.0.1", uint16_t port = 27015, uint16_t listenRate = 1000, uint16_t timeout = 5000, std::function<void(bool)> callback = nullptr);

		static void SendPacket(const char* packet, uint32_t flags = 0x0001);

		static void DisconnectFromServer(uint16_t timeout = 3000, std::function<void()> callback = nullptr);

	private:
		static void ServerListen(uint16_t eventDelay = 1000);
		static bool b_IsServer;
		static _ENetHost* m_netProxy;
		static _ENetPeer* m_netPeer;
		static bool b_IsCreated;
		static bool b_Disconnecting;
		static std::string m_extIPAddress;
		static std::string m_intIPAddress;
		static uint16_t m_port;
		static std::mutex m_ConnectingMutex;
		static std::unique_ptr<Portfwd> m_Portfwd;


	};



}

