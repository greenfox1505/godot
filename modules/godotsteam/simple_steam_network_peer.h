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


protected:
	static void _bind_methods();

public:
	SteamNetworkPeer();
	~SteamNetworkPeer();

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

};


#endif // SIMPLE_STEAM_NETWORK_PEER_H