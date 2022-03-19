#ifndef STEAM_NETWORK_PEER_H
#define STEAM_NETWORK_PEER_H

#include "core/resource.h"
#include "godotsteam.h"
#include "core/io/networked_multiplayer_peer.h"
#include "core/os/os.h"

class SteamNetworkPeer : public NetworkedMultiplayerPeer {
	GDCLASS(SteamNetworkPeer, NetworkedMultiplayerPeer);

	CSteamID lobby_id = CSteamID(uint64(-1));

	CSteamID peerTarget = CSteamID(uint64(-1));
	TransferMode transferMode = TransferMode::TRANSFER_MODE_RELIABLE;

	enum LOBBY_STATE {
		DISCONNECTED = -1,
		HOST_PENDING,
		HOST,
		CLIENT_PENDING,
		CLIENT
	} lobbyState = LOBBY_STATE::DISCONNECTED;


	STEAM_CALLBACK(SteamNetworkPeer, join_requested, GameLobbyJoinRequested_t, callbackJoinRequested);

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
	virtual int get_max_packet_size();
	virtual int get_available_packet_count();

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

	/* Specific to SteamNetworkPeer */
	int get_lobby_state();


	//lobby creation
	uint64 get_lobby_id();
	void create_friend_lobby(int size);
	void create_lobby(int type, int size);
	CCallResult<SteamNetworkPeer, LobbyCreated_t> callResultCreateLobby;
	void lobby_created(LobbyCreated_t *call_data, bool io_failure);
	
	int peer_connected();
	int peer_disconnected();

	Error close_connection();

	void lobby_chat_update(LobbyChatUpdate_t* call_data);

	/*  */
	const uint32_t _gen_unique_id() {
		uint32_t hash = 0;

		while (hash == 0 || hash == 1) {
			hash = hash_djb2_one_32(
					(uint32_t)OS::get_singleton()->get_ticks_usec());
			hash = hash_djb2_one_32(
					(uint32_t)OS::get_singleton()->get_unix_time(), hash);
			hash = hash_djb2_one_32(
					(uint32_t)OS::get_singleton()->get_user_data_dir().hash64(), hash);
			hash = hash_djb2_one_32(
					(uint32_t)((uint64_t)this), hash); // Rely on ASLR heap
			hash = hash_djb2_one_32(
					(uint32_t)((uint64_t)&hash), hash); // Rely on ASLR stack

			hash = hash & 0x7FFFFFFF; // Make it compatible with unsigned, since negative ID is used for exclusion
		}
		return hash;
	}

};


#endif // STEAM_NETWORK_PEER_H