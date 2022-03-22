#include "core/resource.h"
#include "godotsteam.h"
#include "core/io/networked_multiplayer_peer.h"
#include "core/os/os.h"
#include "steam_network_peer.h"

SteamNetworkPeer::SteamNetworkPeer() {
	auto s = Steam::get_singleton();
	// s->set_steam_network_peer(this);
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
		s->connect("steamworks_error",this,"continue");
	}
};
SteamNetworkPeer::~SteamNetworkPeer() {
	auto s = Steam::get_singleton();
	// if ( s->get_steam_network_peer() == this ){
	// 	s->set_steam_network_peer(NULL);
	// }
	//wait, do destructors need to clean this up? doesn't Godot handle this?
	// auto s = Steam::get_singleton();
	// if( s != nullptr){
	// 	s->disconnect( "lobby_message", this,"lobbyMessage");
	// 	s->disconnect( "lobby_chat_update", this,"lobbyChatUpdate");
	// 	s->disconnect( "lobby_created", this,"lobbyCreated");
	// 	s->disconnect( "lobby_data_update", this,"lobbyDataUpdate");
	// 	s->disconnect( "lobby_joined", this,"lobbyJoined");
	// 	s->disconnect( "lobby_game_created", this,"lobbyGameCreated");
	// 	s->disconnect( "lobby_invite", this,"lobbyInvite");
	// 	s->disconnect( "lobby_match_list", this,"lobbyMatchList");
	// 	s->disconnect( "lobby_kicked", this,"lobbyKicked");
	// }
};

void SteamNetworkPeer::_bind_methods() {
	// ClassDB::bind_method(D_METHOD())
	//I hate this. None of the fucntions should be called externally...
	ClassDB::bind_method(D_METHOD("lobbyMessage"), &SteamNetworkPeer::lobbyMessage);
	ClassDB::bind_method(D_METHOD("lobbyChatUpdate"), &SteamNetworkPeer::lobbyChatUpdate);
	ClassDB::bind_method(D_METHOD("lobbyCreated"), &SteamNetworkPeer::lobbyCreated);
	ClassDB::bind_method(D_METHOD("lobbyDataUpdate"), &SteamNetworkPeer::lobbyDataUpdate);
	ClassDB::bind_method(D_METHOD("lobbyJoined"), &SteamNetworkPeer::lobbyJoined);
	ClassDB::bind_method(D_METHOD("lobbyGameCreated"), &SteamNetworkPeer::lobbyGameCreated);
	ClassDB::bind_method(D_METHOD("lobbyInvite"), &SteamNetworkPeer::lobbyInvite);
	ClassDB::bind_method(D_METHOD("lobbyMatchList"), &SteamNetworkPeer::lobbyMatchList);
	ClassDB::bind_method(D_METHOD("lobbyKicked"), &SteamNetworkPeer::lobbyKicked);

	ClassDB::bind_method(D_METHOD("create_lobby", "lobby_type", "max_members"), &SteamNetworkPeer::createLobby, DEFVAL(FRIENDS_ONLY),DEFVAL(2));

	ADD_SIGNAL(MethodInfo("continue", PropertyInfo(Variant::STRING, "message")));
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
	Steam::get_singleton()->createLobby(lobby_type,max_members);
}

/* Specific to PacketPeer */
Error SteamNetworkPeer::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {
	WARN_PRINT("SteamNetworkPeer::get_packet: not yet implemented!");
	return Error::ERR_PRINTER_ON_FIRE;
};

Error SteamNetworkPeer::put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	WARN_PRINT("SteamNetworkPeer::put_packet: not yet implemented!");
	return Error::ERR_PRINTER_ON_FIRE;
};

int SteamNetworkPeer::get_max_packet_size() const {
	WARN_PRINT("SteamNetworkPeer::get_max_packet_size: not yet implemented!");
	return -1;
};

int SteamNetworkPeer::get_available_packet_count() const {
	return this->receivedPackets.size();
	// WARN_PRINT("SteamNetworkPeer::get_available_packet_count: not yet implemented!");
	// return -1;
};


/* Specific to NetworkedMultiplayerPeer */
void SteamNetworkPeer::set_transfer_mode(NetworkedMultiplayerPeer::TransferMode p_mode) {
	WARN_PRINT("SteamNetworkPeer::set_transfer_mode: not yet implemented!");
	return;
};

NetworkedMultiplayerPeer::TransferMode SteamNetworkPeer::get_transfer_mode() const {
	WARN_PRINT("SteamNetworkPeer::get_transfer_mode: not yet implemented!");
	return NetworkedMultiplayerPeer::TransferMode::TRANSFER_MODE_RELIABLE;
};

void SteamNetworkPeer::set_target_peer(int p_peer_id) {
	WARN_PRINT("SteamNetworkPeer::set_target_peer: not yet implemented!");
	return;
};

int SteamNetworkPeer::get_packet_peer() const {
	WARN_PRINT("SteamNetworkPeer::get_packet_peer: not yet implemented!");
	return -1;
};

bool SteamNetworkPeer::is_server() const {
	WARN_PRINT("SteamNetworkPeer::is_server: not yet implemented!");
	return false;
};

void SteamNetworkPeer::poll() {
	WARN_PRINT("SteamNetworkPeer::poll: not yet implemented!");
	return;
};

int SteamNetworkPeer::get_unique_id() const {
	WARN_PRINT("SteamNetworkPeer::get_unique_id: not yet implemented!");
	return -1;
};

void SteamNetworkPeer::set_refuse_new_connections(bool p_enable) {
	WARN_PRINT("SteamNetworkPeer::set_refuse_new_connections: not yet implemented!");
	return;
};

bool SteamNetworkPeer::is_refusing_new_connections() const {
	WARN_PRINT("SteamNetworkPeer::is_refusing_new_connections: not yet implemented!");
	return false;
};

NetworkedMultiplayerPeer::ConnectionStatus SteamNetworkPeer::get_connection_status() const {
	WARN_PRINT("SteamNetworkPeer::ConnectionStatus: not yet implemented!");
	return NetworkedMultiplayerPeer::ConnectionStatus::CONNECTION_CONNECTED;
};



/* Callbacks */
void SteamNetworkPeer::lobbyMessage( int lobbyId, int user, String message, int chatType){
	WARN_PRINT("lobbyMessage: not yet implemented!");
};

void SteamNetworkPeer::lobbyChatUpdate( int lobbyId, int changedId, int makingChangeId, int chatState){
	WARN_PRINT("lobbyChatUpdate: not yet implemented!");
};

void SteamNetworkPeer::lobbyCreated( int connect, int lobbyId){
	WARN_PRINT("lobbyCreated: not yet implemented!");
};

void SteamNetworkPeer::lobbyDataUpdate( int success, int lobbyId, int memberId){
	WARN_PRINT("lobbyDataUpdate: not yet implemented!");
};

void SteamNetworkPeer::lobbyJoined( int lobby, int permissions, bool locked, int response){
	WARN_PRINT("lobbyJoined: not yet implemented!");
};

void SteamNetworkPeer::lobbyGameCreated( int lobbyId, int serverId, String serverIp, int port){
	WARN_PRINT("lobbyGameCreated: not yet implemented!");
};

void SteamNetworkPeer::lobbyInvite( int inviter, int lobby, int game){
	WARN_PRINT("lobbyInvite: not yet implemented!");
};

void SteamNetworkPeer::lobbyMatchList( Array lobbies){
	WARN_PRINT("lobbyMatchList: not yet implemented!");
};

void SteamNetworkPeer::lobbyKicked( int lobbyId, int adminId, int dueToDisconnect){
	WARN_PRINT("lobbyKicked: not yet implemented!");
};







