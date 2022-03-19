#include "core/resource.h"
#include "godotsteam.h"
#include "core/io/networked_multiplayer_peer.h"
#include "core/os/os.h"
#include "steam_network_peer.h"

SteamNetworkPeer::SteamNetworkPeer() {
	auto s = Steam::get_singleton();
	if( s != nullptr){
		s->connect( "lobby_message", this,"lobbyMessage");
		s->connect( "lobby_chat_update", this,"lobbyChatUpdate");
		s->connect( "lobby_created", this,"lobbyCreated");
		s->connect( "lobby_data_update", this,"lobbyDataUpdate");
		s->connect( "lobby_joined", this,"lobbyJoined");
		s->connect( "lobby_game_created", this,"lobbyGameCreated");
		s->connect( "lobby_invite", this,"lobbyInvite");
		s->connect( "lobby_match_list", this,"lobbyMatchList");
		s->connect( "lobby_kicked", this,"lobbyKicked");
	}
};
SteamNetworkPeer::~SteamNetworkPeer() {
	auto s = Steam::get_singleton();
	if( s != nullptr){
		s->disconnect( "lobby_message", this,"lobbyMessage");
		s->disconnect( "lobby_chat_update", this,"lobbyChatUpdate");
		s->disconnect( "lobby_created", this,"lobbyCreated");
		s->disconnect( "lobby_data_update", this,"lobbyDataUpdate");
		s->disconnect( "lobby_joined", this,"lobbyJoined");
		s->disconnect( "lobby_game_created", this,"lobbyGameCreated");
		s->disconnect( "lobby_invite", this,"lobbyInvite");
		s->disconnect( "lobby_match_list", this,"lobbyMatchList");
		s->disconnect( "lobby_kicked", this,"lobbyKicked");
	}
};

void SteamNetworkPeer::_bind_methods() {

	ClassDB::bind_method(D_METHOD("createLobby", "lobby_type", "max_members"), &Steam::createLobby, DEFVAL(2));

	ADD_SIGNAL(MethodInfo("lobbyEvent", PropertyInfo(Variant::STRING, "message")));

	BIND_CONSTANT(DISCONNECTED);
	BIND_CONSTANT(HOST_PENDING);
	BIND_CONSTANT(HOST);
	BIND_CONSTANT(CLIENT_PENDING);
	BIND_CONSTANT(CLIENT);
	BIND_CONSTANT(PRIVATE);
	BIND_CONSTANT(FRIENDS_ONLY);
	BIND_CONSTANT(PUBLIC);
	BIND_CONSTANT(INVISIBLE);
	BIND_CONSTANT(PRIVATE_UNIQUE);
};

/* User Functions */
void SteamNetworkPeer::createLobby(int lobby_type, int max_members){

}

/* Specific to PacketPeer */
Error SteamNetworkPeer::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {
	return Error::ERR_PRINTER_ON_FIRE;
};

Error SteamNetworkPeer::put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	return Error::ERR_PRINTER_ON_FIRE;
};

int SteamNetworkPeer::get_max_packet_size() const {
	return -1;
};

int SteamNetworkPeer::get_available_packet_count() const {
	return -1;
};


/* Specific to NetworkedMultiplayerPeer */
void SteamNetworkPeer::set_transfer_mode(NetworkedMultiplayerPeer::TransferMode p_mode) {
	return;
};

NetworkedMultiplayerPeer::TransferMode SteamNetworkPeer::get_transfer_mode() const {
	return NetworkedMultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE;
};

void SteamNetworkPeer::set_target_peer(int p_peer_id) {
	return;
};

int SteamNetworkPeer::get_packet_peer() const {
	return -1;
};

bool SteamNetworkPeer::is_server() const {
	return false;
};

void SteamNetworkPeer::poll() {
	return;
};

int SteamNetworkPeer::get_unique_id() const {
	return -1;
};

void SteamNetworkPeer::set_refuse_new_connections(bool p_enable) {
	return;
};

bool SteamNetworkPeer::is_refusing_new_connections() const {
	return false;
};

NetworkedMultiplayerPeer::ConnectionStatus SteamNetworkPeer::get_connection_status() const {
	return NetworkedMultiplayerPeer::ConnectionStatus::CONNECTION_CONNECTED;
};



/* Callbacks */
void SteamNetworkPeer::lobbyMessage( int lobbyId, int user, String message, int chatType){

};

void SteamNetworkPeer::lobbyChatUpdate( int lobbyId, int changedId, int makingChangeId, int chatState){

};

void SteamNetworkPeer::lobbyCreated( int connect, int lobbyId){

};

void SteamNetworkPeer::lobbyDataUpdate( int success, int lobbyId, int memberId){

};

void SteamNetworkPeer::lobbyJoined( int lobby, int permissions, bool locked, int response){

};

void SteamNetworkPeer::lobbyGameCreated( int lobbyId, int serverId, String serverIp, int port){

};

void SteamNetworkPeer::lobbyInvite( int inviter, int lobby, int game){

};

void SteamNetworkPeer::lobbyMatchList( Array lobbies){

};

void SteamNetworkPeer::lobbyKicked( int lobbyId, int adminId, int dueToDisconnect){

};







