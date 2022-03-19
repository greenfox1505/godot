#include "steam_network_peer.h"
#include "core/error_macros.h"
// #include "class_db.h"

#define SNP_CONTINUE "continue" 

void SteamNetworkPeer::_bind_methods() {

	ClassDB::bind_method(D_METHOD("create_friend_lobby", "max_members"), &SteamNetworkPeer::create_friend_lobby, DEFVAL(2));
	ClassDB::bind_method(D_METHOD("get_lobby_id"), &SteamNetworkPeer::get_lobby_id);
	ClassDB::bind_method(D_METHOD("close_connection"), &SteamNetworkPeer::close_connection);
	ClassDB::bind_method(D_METHOD("get_lobby_state"), &SteamNetworkPeer::get_lobby_state);
	ClassDB::bind_method(D_METHOD("peer_connected"), &SteamNetworkPeer::peer_connected);

	//LOBBY_STATE enums
	BIND_CONSTANT(DISCONNECTED);
	BIND_CONSTANT(HOST_PENDING);
	BIND_CONSTANT(HOST);
	BIND_CONSTANT(CLIENT_PENDING);
	BIND_CONSTANT(CLIENT);

	//ELobbyType proxy
	BIND_CONSTANT(PRIVATE);
	BIND_CONSTANT(FRIENDS_ONLY);
	BIND_CONSTANT(PUBLIC);
	BIND_CONSTANT(INVISIBLE);
	BIND_CONSTANT(PRIVATE_UNIQUE);



	ADD_SIGNAL(MethodInfo(SNP_CONTINUE, PropertyInfo(Variant::STRING, "MSG")));
}


SteamNetworkPeer::SteamNetworkPeer(): 
	callbackJoinRequested(this, &SteamNetworkPeer::join_requested)
{
	auto steam = Steam::get_singleton();
	steam->connect("lobby_chat_update", this, "_on_lobby_chat_update");
	//todo verify steam is running
	// CRASH_COND_MSG(Steam::get_singleton()->is_init_success == false, "steam must be init'd for a peer to create");
	// Steam::get_singleton()->connect("",this,"");
}

SteamNetworkPeer::~SteamNetworkPeer() {
}

//first godot gets the sender id via `get_packet_peer`, then it gets packet data via `get_packet`
int SteamNetworkPeer::get_packet_peer() const {
	return -1;
}

Error SteamNetworkPeer::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {
	return ERR_PRINTER_ON_FIRE;
}

Error SteamNetworkPeer::put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	// Steam::sendP2PPacket;
	return ERR_PRINTER_ON_FIRE;
}

int SteamNetworkPeer::get_max_packet_size() const {
	return ERR_PRINTER_ON_FIRE;
}

int SteamNetworkPeer::get_available_packet_count() const {
	return 0;
	// return ERR_PRINTER_ON_FIRE;
}

void SteamNetworkPeer::set_transfer_mode(TransferMode transferMode) {
	this->transferMode = transferMode;
}
NetworkedMultiplayerPeer::TransferMode SteamNetworkPeer::get_transfer_mode() const {
	return this->transferMode;
}
void SteamNetworkPeer::set_target_peer(int p_peer_id) {
	// peerTarget = p_peer_id;
}


bool SteamNetworkPeer::is_server() const {
	
	return false;
}

void SteamNetworkPeer::poll() {
}

int SteamNetworkPeer::get_unique_id() const {
	return -1;
}

void SteamNetworkPeer::set_refuse_new_connections(bool p_enable) {
}
bool SteamNetworkPeer::is_refusing_new_connections() const {
	return false;
}

NetworkedMultiplayerPeer::ConnectionStatus SteamNetworkPeer::get_connection_status() const {
	// auto a = Steam::get_singleton()->getP2PSessionState();

	if( lobbyState == DISCONNECTED ){
		return NetworkedMultiplayerPeer::ConnectionStatus::CONNECTION_DISCONNECTED;
	}
	else if (lobbyState == HOST_PENDING && lobbyState == CLIENT_PENDING){
		return NetworkedMultiplayerPeer::ConnectionStatus::CONNECTION_CONNECTING;
	}
	else {
		return NetworkedMultiplayerPeer::ConnectionStatus::CONNECTION_CONNECTED;
	}

}

int SteamNetworkPeer::get_lobby_state(){
	return lobbyState;
}


uint64 SteamNetworkPeer::get_lobby_id(){
	return lobby_id.ConvertToUint64();
}

void SteamNetworkPeer::create_friend_lobby(int size) {
	int type = LOBBY_TYPE::FRIENDS_ONLY;
	create_lobby(type,size);
}
void SteamNetworkPeer::create_lobby(int type, int size){
	if(SteamMatchmaking() != NULL){
		lobbyState = LOBBY_STATE::HOST_PENDING;
		SteamAPICall_t api_call = SteamMatchmaking()->CreateLobby(ELobbyType(type), size);
		callResultCreateLobby.Set(api_call, this, &SteamNetworkPeer::lobby_created);
	}
	else{
		emit_signal(SNP_CONTINUE,"Steam not init'd!");
	}
}
void SteamNetworkPeer::lobby_created(LobbyCreated_t *lobbyData, bool io_failure){
	if(io_failure){
		lobbyState = LOBBY_STATE::DISCONNECTED;
		emit_signal(SNP_CONTINUE,"fail");
		// emit_signal(ERROR_MSG,"io failure?");
	}
	else {
		lobbyState = LOBBY_STATE::HOST;
		int connect = lobbyData->m_eResult;
		lobby_id = lobbyData->m_ulSteamIDLobby;
		emit_signal(SNP_CONTINUE,lobbyData->m_ulSteamIDLobby);
	}
}


Error SteamNetworkPeer::close_connection(){
	return ERR_PRINTER_ON_FIRE;
}


//! A lobby chat room state has changed, this is usually sent when a user has joined or left the lobby.
void SteamNetworkPeer::lobby_chat_update(LobbyChatUpdate_t* call_data){
	uint64_t lobby_id = call_data->m_ulSteamIDLobby;
	uint64_t changed_id = call_data->m_ulSteamIDUserChanged;
	uint64_t making_change_id = call_data->m_ulSteamIDMakingChange;
	uint32 chat_state = call_data->m_rgfChatMemberStateChange;
	// emit_signal("lobby_chat_update", lobby_id, changed_id, making_change_id, chat_state);
}

int SteamNetworkPeer::peer_connected(){
	return -1;
}
int SteamNetworkPeer::peer_disconnected(){
	return -1;
}


void SteamNetworkPeer::join_requested(GameLobbyJoinRequested_t* call_data){
	CSteamID lobby_id = call_data->m_steamIDLobby;
	uint64_t lobby = lobby_id.ConvertToUint64();
	CSteamID friend_id = call_data->m_steamIDFriend;
	uint64_t steam_id = friend_id.ConvertToUint64();
	emit_signal("join_requested", lobby, steam_id);
}

// int SteamNetworkPeer::create_lobby(Steam::LobbyType lobby_type,uint64 max_members){
// 	return -1;
// }




/*

var s = SteamNetworkPeer.new()
get_tree().network_peer = s
s.create_server(type,count)
var y = yield(s,"lobbyCreate")



*/
