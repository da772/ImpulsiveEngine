#include "gepch.h"

#include "Public/Core/Networking/Networking.h"
#include "Public/Core/Util/Utility.h"
#include "Public/Core/Util/ThreadPool.h"

#include <enet/enet.h>
#include <httplib.h>

#include <miniupnpc/portfwd.h>

namespace GEngine {

	bool Networking::b_IsCreated = false;
	bool Networking::b_IsServer = false;
	_ENetHost* Networking::m_netProxy = nullptr;
	_ENetPeer* Networking::m_netPeer = nullptr;
	std::string Networking::m_extIPAddress = "127.0.0.1";
	std::string Networking::m_intIPAddress = "127.0.0.1";
	uint16_t Networking::m_port = 27015;
	std::mutex Networking::m_ConnectingMutex;
	bool Networking::b_Disconnecting = false;
	std::unique_ptr<Portfwd> Networking::m_Portfwd = std::make_unique<Portfwd>();
	

	void Networking::Init()
	{
		enet_initialize();
	}

	void Networking::Shutdown()
	{
		Reset();
		enet_deinitialize();
	}

	void Networking::Reset()
	{
		if (b_IsCreated) {
			if (!Networking::b_Disconnecting) {
				DisconnectFromServer(3000, nullptr);
			}
			Networking::m_Portfwd = nullptr;
			Networking::m_netProxy = nullptr;
			Networking::b_IsServer = false;
			Networking::b_IsCreated = false;
			Networking::m_netPeer = nullptr;
			Networking::b_Disconnecting = false;
		}
	}

	void Networking::CreateServer(uint16_t port /*= 27000*/, uint16_t listenRate, unsigned char maxClient /*= 32*/,  unsigned char channels /*= 2*/, uint32_t incomingBandwith /*= 0*/, uint32_t outgoingBandwidth /*= 0*/)
	{
		std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
		GE_CORE_ASSERT(!Networking::b_IsCreated, "NETWORK ALREADY SETUP CALL Networking::Reset() BEFORE THIS CALL");

		if (!b_IsCreated) {
			
			ThreadPool::AddJob([listenRate, port, maxClient, channels, incomingBandwith, outgoingBandwidth]() {
				{
					
					ENetAddress address;
					address.host = ENET_HOST_ANY;
					address.port = port;
					{
						std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
						if (Networking::m_Portfwd == nullptr) {
							m_Portfwd = std::make_unique<Portfwd>();
						}
					}

					if (Networking::m_Portfwd->init(2000)) {
						Networking::m_Portfwd->remove(port);
						Networking::m_Portfwd->add(port);
						Networking::m_port = port;
					}
					else {
						m_Portfwd = nullptr;
						GE_CORE_ERROR("Cannot initialize UPNP");
						return;
					}
					{
						std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
						Networking::m_netProxy = enet_host_create(&address, maxClient, channels, incomingBandwith, outgoingBandwidth);
						Networking::m_extIPAddress = Networking::m_Portfwd->external_ip();
						Networking::m_intIPAddress = Networking::m_Portfwd->lan_ip();
						GE_CORE_INFO("External IP: {0} Internal IP: {1} Port: {2}", Networking::m_extIPAddress, Networking::m_intIPAddress,
							Networking::m_port);
					}
					
					{
						std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
						if (Networking::m_netProxy == NULL) {
							GE_CORE_ASSERT(false, "FAILED TO CREATE NETWORK");
						}
						Networking::b_IsServer = true;
						Networking::b_IsCreated = true;
					}

				}

				httplib::Client cli("api.ipify.org");
				auto res = cli.Get("/");
				
				if (res != NULL) {
					std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
					std::string body = res->body;
					Networking::m_extIPAddress = body;
					GE_CORE_DEBUG("HOST IP SET: {0}", body);
				}
				else {
					GE_CORE_ERROR("CANNOT CONNECT TO INTERNET");
				}

				ServerListen(listenRate);
			});

		}

	}

	void Networking::CreateClient()
	{
		std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
		GE_CORE_ASSERT(!Networking::b_IsCreated, "NETWORK ALREADY SETUP CALL Networking::Reset() BEFORE THIS CALL");

		if (!Networking::b_IsCreated) {
			m_netProxy = enet_host_create(NULL, 1, 2, 0, 0);

			if (Networking::m_netProxy == NULL) {
				GE_CORE_ASSERT(false, "FAILED TO CREATE NETWORK");
			}

			Networking::b_IsCreated = true;
		}
	}

	void Networking::ConnectToServer(std::string ip, uint16_t port, uint16_t listenRate, uint16_t timeout, std::function<void(bool)> callback)
	{
		
		CreateClient();
		std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
		GE_CORE_ASSERT(Networking::b_IsCreated && !Networking::b_IsServer, "NETWORK ALREADY SETUP CALL Networking::Reset() BEFORE THIS CALL");

		if (Networking::b_IsCreated && !Networking::b_IsServer) {

			Networking::m_extIPAddress = ip;
			Networking::m_port = port;
			
			ThreadPool::AddJob([ip, port, listenRate, timeout, callback]() {
				
				ENetAddress address;
				ENetEvent event;
				int service = 0;
				{
					std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);

					enet_address_set_host(&address, ip.c_str());
					address.port = port;
					Networking::m_netPeer = enet_host_connect(Networking::m_netProxy, &address, 2, 0);

					if (Networking::m_netPeer == NULL) {
						GE_CORE_ERROR("NO AVAILABLE PEERS");
						return;
					}
						
					service = enet_host_service(Networking::m_netProxy, &event, timeout);
					
				}

				if (service > 0) {
					GE_CORE_INFO("Connected to {0}:{1}", ip, port);
					if (callback != nullptr) {
						callback(true);
					}
					ServerListen(listenRate);
					return;
				} else {
					std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
					GE_CORE_INFO("Could not connect to {0}:{1}", ip, port);
					enet_peer_reset(Networking::m_netPeer);
					Networking::m_netPeer = nullptr;
					Networking::b_Disconnecting = true;
					Networking::Reset();
					if (callback != nullptr) {
						callback(false);
					}
					return;
				}
			});
		}
	}

	void Networking::SendPacket(const char* packet, uint32_t flags /*= 0*/)
	{
		std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);

		ENetPacket* ePacket = enet_packet_create(packet, strlen(packet) + 1, flags);
		enet_host_broadcast(Networking::m_netProxy, 0, ePacket);
		enet_host_flush(Networking::m_netProxy);
		
	}

	void Networking::ServerListen(uint16_t eventDelay)
	{
		bool canListen = false;
		bool isServer = false;
		{
			std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
			canListen = Networking::b_IsCreated;
			isServer = Networking::b_IsServer;
		}
		
		if (canListen) {
			ENetHost* host = NULL;
			{
				std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
				host = Networking::m_netProxy;
			}
			ENetEvent event;

			long eventType = enet_host_service(host, &event, eventDelay);

			while (host != NULL && eventType >= 0) {
				ENetPeer* peer = NULL;
				if (!isServer) {
					
					{
						std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
						peer = Networking::m_netPeer;
					}
					if (peer != NULL && peer->state == ENetPeerState::ENET_PEER_STATE_DISCONNECTED) {
						DisconnectFromServer();
						GE_CORE_DEBUG("Disconnected From Server...");
						return;
					}
				}
				switch (event.type) {
				case 0:
					break;
				case ENET_EVENT_TYPE_CONNECT: {	
					GE_CORE_DEBUG("A new client connected from {0}:{1}, id:{2}", Utility::IPV4ToString(event.peer->address.host), event.peer->address.port,event.peer->connectID);

					break;
				}
				case ENET_EVENT_TYPE_RECEIVE: {
					GE_CORE_DEBUG("Received packed of length {0}, containing {1}, from {2} on channel {3}", event.packet->dataLength,
						(char*)event.packet->data, event.peer->connectID, event.channelID);
					{
						if (Networking::b_IsServer) {
							std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
							for (int i = 0; i < Networking::m_netProxy->connectedPeers; i++) {
								ENetPacket* sPacket = enet_packet_create(event.packet->data, event.packet->dataLength, 0);
								ENetPeer* p = &Networking::m_netProxy->peers[i];
								if (p != nullptr && p != event.peer)
									enet_peer_send(p, 0x0001, sPacket);
								enet_host_flush(Networking::m_netProxy);
							}
							
						}
					}
					break;
				}
				case ENET_EVENT_TYPE_DISCONNECT:
				{
					std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
					GE_CORE_DEBUG("Client disconnected, {0}:{1}, id:{2}", Utility::IPV4ToString(event.peer->address.host), event.peer->address.port, event.peer->connectID);
					event.peer->data = NULL;
					break;
				}
				default:
					GE_CORE_DEBUG("UNKNOWN PACKET RECIEVED!");
					break;
				}

				eventType = enet_host_service(host, &event, eventDelay);
			}
		}

	}

	void Networking::DisconnectFromServer(uint16_t timeout, std::function<void()> callback)
	{

		{
			std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
			if (!Networking::b_IsCreated) {
				return;
			}
		}

		ThreadPool::AddJob([timeout, callback]()
			{
				std::lock_guard<std::mutex> guard(Networking::m_ConnectingMutex);
				Networking::b_Disconnecting = true;
				if (Networking::b_IsCreated && !Networking::b_IsServer && Networking::m_netPeer != nullptr) {

					ENetHost* h = NULL;
					ENetPeer* p = NULL;
					{

						h = Networking::m_netProxy;
						p = Networking::m_netPeer;
					}

					ENetEvent event;
					if (p != NULL) {
						enet_peer_disconnect(p, 0);
						enet_peer_reset(p);
						p = NULL;
					}
					Networking::Reset();
					if (callback != nullptr)
						callback();
					return;
				}
				else {

					bool serv = false;
					ENetHost* host = NULL;
					{
						serv = Networking::b_IsServer;
						host = Networking::m_netProxy;

					}
					if (serv && host != NULL) {
						enet_host_flush(host);
						enet_host_destroy(host);
                        Networking::m_Portfwd->remove(Networking::m_port);
						Networking::Reset();
						if (callback != nullptr)
							callback();
						return;
					}
				}
               
				Networking::Reset();
				if (callback != nullptr)
					callback();
			});

		

		

	}

}
