#ifndef SIMPLE_STEAM_NETWORK_PEER_H
#define SIMPLE_STEAM_NETWORK_PEER_H

#include "core/resource.h"
#include "godotsteam.h"
#include "core/io/networked_multiplayer_peer.h"
#include "core/os/os.h"
#include "steam/steam_api.h"

#define MAX_STEAM_PACKET_SIZE 4096
class SteamNetworkPeer : public NetworkedMultiplayerPeer {
public:

	typedef NetworkedMultiplayerPeer::ConnectionStatus ConnectionStatus;
	typedef NetworkedMultiplayerPeer::TransferMode TransferMode;

private:
	GDCLASS(SteamNetworkPeer, NetworkedMultiplayerPeer);
	
	Steam* steam;

	enum LOBBY_STATE {
		DISCONNECTED = -1,
		HOST_PENDING,
		HOST,
		CLIENT_PENDING,
		CLIENT
	} lobbyState = LOBBY_STATE::DISCONNECTED;

	enum class CHAT_CHANGE {
		ENTERED = k_EChatMemberStateChangeEntered,
		LEFT = k_EChatMemberStateChangeLeft,
		DISCONNECTED = k_EChatMemberStateChangeDisconnected,
		KICKED = k_EChatMemberStateChangeKicked,
		BANNED = k_EChatMemberStateChangeBanned
	};

	struct Packet {
		uint8_t data[MAX_STEAM_PACKET_SIZE];
		int size;
		CSteamID sender;
		int channel;
	};
	Packet* lastPacket = new Packet;
	List<Packet*> receivedPackets = List<Packet*>();
	// List<Packet> sentPackets;

	ConnectionStatus connectionStatus = ConnectionStatus::CONNECTION_DISCONNECTED;
	TransferMode transferMode = TransferMode::TRANSFER_MODE_RELIABLE;
	int targetPeer = 0; //0 means all

	CSteamID lobbyId;
	bool isServer = false;
	bool refuseConnections = false;

	Dictionary lobbyData;
	List<uint32> playerList;

	void updateLobbyData();

protected:
	static void _bind_methods();

public:
	SteamNetworkPeer();
	~SteamNetworkPeer();



	/* User Functions */
	void createServer(int lobby_type, int max_members);
	void createClient(uint64_t lobbyId);

	enum LOBBY_TYPE {
		PRIVATE = ELobbyType::k_ELobbyTypePrivate,
		FRIENDS_ONLY = ELobbyType::k_ELobbyTypeFriendsOnly,
		PUBLIC = ELobbyType::k_ELobbyTypePublic,
		INVISIBLE = ELobbyType::k_ELobbyTypeInvisible,
		PRIVATE_UNIQUE = ELobbyType::k_ELobbyTypePrivateUnique,
	};

	/* Specific to PacketPeer */
	virtual Error get_packet(const uint8_t **r_buffer, int &r_buffer_size);
	virtual Error put_packet(const uint8_t *p_buffer, int p_buffer_size);
	virtual int get_max_packet_size() const;
	virtual int get_available_packet_count() const;

	/* Specific to NetworkedMultiplayerPeer */
	virtual void set_transfer_mode(TransferMode p_mode);
	virtual TransferMode get_transfer_mode() const;
	virtual void set_target_peer(int p_peer_id);
	virtual int get_packet_peer() const;
	virtual bool is_server() const;
	virtual void poll();
	virtual int get_unique_id() const;
	virtual void set_refuse_new_connections(bool p_enable);
	virtual bool is_refusing_new_connections() const;
	virtual ConnectionStatus get_connection_status() const;

	/* Callbacks */
	void lobbyChatUpdate( uint64_t lobbyId, uint64_t changedId, uint64_t makingChangeId, uint32 chatState);
	void lobbyCreated( int connect, uint64_t lobbyId);
	void lobbyDataUpdate( uint8 success, uint64_t lobbyId, uint64_t memberId);
	void lobbyJoined( uint64_t lobbyId, uint32_t permissions, bool locked, uint32_t response);
	void lobbyGameCreated( uint64_t lobbyId, uint64_t serverId, String serverIp, uint16 port);
	void lobbyInvite( uint64_t inviter, uint64_t lobbyId, uint64_t game);
	void lobbyMatchList( Array lobbies);
	void lobbyKicked( uint64_t lobbyId, uint64_t adminId, uint8 dueToDisconnect);
	void joinGameRequested(uint64_t lobbyId, String connect);
	void joinRequested(int lobby,int steam_id);

	STEAM_CALLBACK(SteamNetworkPeer, lobbyMessage, LobbyChatMsg_t, callbackLobbyMessage);
	STEAM_CALLBACK(SteamNetworkPeer, lobbyChatUpdate, LobbyChatUpdate_t, callbackLobbyChatUpdate);

	// STEAM_CALLBACK(SteamNetworkPeer, joinRequested, GameLobbyJoinRequested_t, callbackJoinRequested);
	// STEAM_CALLBACK(SteamNetworkPeer, lobbyMessage, LobbyChatMsg_t, callbackLobbyMessage);
	// void lobbyMessage( uint64_t lobbyId, uint64_t user, PoolByteArray message, uint8 chatType);
};

#endif // SIMPLE_STEAM_NETWORK_PEER_H