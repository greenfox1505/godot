#ifndef SIMPLE_STEAM_NETWORK_PEER_H
#define SIMPLE_STEAM_NETWORK_PEER_H

#include "core/resource.h"
#include "godotsteam.h"
#include "core/io/networked_multiplayer_peer.h"
#include "core/os/os.h"

class SteamNetworkPeer : public NetworkedMultiplayerPeer {
	GDCLASS(SteamNetworkPeer, NetworkedMultiplayerPeer);
	
	enum LOBBY_STATE {
		DISCONNECTED = -1,
		HOST_PENDING,
		HOST,
		CLIENT_PENDING,
		CLIENT
	} lobbyState = LOBBY_STATE::DISCONNECTED;

	struct Packet {
		void* data;
		uint32 size;
		uint64 from;
		int channel;
	};

	List<Packet> receivedPackets;
	// List<Packet> sentPackets;

protected:
	static void _bind_methods();

public:
	SteamNetworkPeer();
	~SteamNetworkPeer();

	/* User Functions */
	void createLobby(int lobby_type, int max_members);

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

	// void lobbyCreated();
	// void lobbyMatchList();
	// void lobbyJoined();
	// void lobbyChatUpdate();
	// void lobbyMessage();
	// void lobbyDataUpdate(uint64_t lobby_id, uint64_t changed_id, uint64_t making_change_id, uint32 chat_state);
	// void lobbyInvite();
	// void joinRequested();

	
	void lobbyMessage( int lobbyId, int user, String message, int chatType);
	void lobbyChatUpdate( int lobbyId, int changedId, int makingChangeId, int chatState);
	void lobbyCreated( int connect, int lobbyId);
	void lobbyDataUpdate( int success, int lobbyId, int memberId);
	void lobbyJoined( int lobby, int permissions, bool locked, int response);
	void lobbyGameCreated( int lobbyId, int serverId, String serverIp, int port);
	void lobbyInvite( int inviter, int lobby, int game);
	void lobbyMatchList( Array lobbies);
	void lobbyKicked( int lobbyId, int adminId, int dueToDisconnect);
};


#endif // SIMPLE_STEAM_NETWORK_PEER_H