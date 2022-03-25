#include "core/resource.h"
#include "godotsteam.h"
#include "core/io/networked_multiplayer_peer.h"
#include "core/os/os.h"
#include "steam_network_peer.h"

#define IS_DEBUG true
#if IS_DEBUG
#define TODO_PRINT(str) WARN_PRINT(String("TODO:") + String(str))
#else
#define TODO_PRINT(ignored)
#endif


SteamNetworkPeer::SteamNetworkPeer():
	callbackLobbyMessage(this, &SteamNetworkPeer::lobbyMessage)
{
	steam = Steam::get_singleton();
	// s->set_steam_network_peer(this);
	if( steam != nullptr){
		steam->connect( "lobby_chat_update", this,"lobbyChatUpdate");
		steam->connect( "lobby_created", this,"lobbyCreated");
		steam->connect( "lobby_data_update", this,"lobbyDataUpdate");
		steam->connect( "lobby_joined", this,"lobbyJoined");
		steam->connect( "lobby_game_created", this,"lobbyGameCreated");
		steam->connect( "lobby_invite", this,"lobbyInvite");
		steam->connect( "lobby_match_list", this,"lobbyMatchList");
		steam->connect( "lobby_kicked", this,"lobbyKicked");
		steam->connect("steamworks_error",this,"continue");
	}
	this->connectionStatus = ConnectionStatus::CONNECTION_DISCONNECTED;
};
SteamNetworkPeer::~SteamNetworkPeer()
{

};

void SteamNetworkPeer::_bind_methods() {
	// ClassDB::bind_method(D_METHOD())
	//I hate this. None of the fucntions should be called externally...
	ClassDB::bind_method(D_METHOD("lobbyChatUpdate"), &SteamNetworkPeer::lobbyChatUpdate);
	ClassDB::bind_method(D_METHOD("lobbyCreated"), &SteamNetworkPeer::lobbyCreated);
	ClassDB::bind_method(D_METHOD("lobbyDataUpdate"), &SteamNetworkPeer::lobbyDataUpdate);
	ClassDB::bind_method(D_METHOD("lobbyJoined"), &SteamNetworkPeer::lobbyJoined);
	ClassDB::bind_method(D_METHOD("lobbyGameCreated"), &SteamNetworkPeer::lobbyGameCreated);
	ClassDB::bind_method(D_METHOD("lobbyInvite"), &SteamNetworkPeer::lobbyInvite);
	ClassDB::bind_method(D_METHOD("lobbyMatchList"), &SteamNetworkPeer::lobbyMatchList);
	ClassDB::bind_method(D_METHOD("lobbyKicked"), &SteamNetworkPeer::lobbyKicked);

	ClassDB::bind_method(D_METHOD("create_lobby", "lobby_type", "max_members"), &SteamNetworkPeer::createLobby, DEFVAL(FRIENDS_ONLY),DEFVAL(2));

	// peer_connected //pre existing signals
	// peer_disconnected
	// connection_succeeded
	// connection_failed
	// server_disconnected

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
	connectionStatus = ConnectionStatus::CONNECTION_CONNECTING;
	Steam::get_singleton()->createLobby(lobby_type,max_members);
}

/* Specific to PacketPeer */
Error SteamNetworkPeer::get_packet(const uint8_t **r_buffer, int &r_buffer_size) {
	delete lastPacket;
	lastPacket = receivedPackets.front()->get();
	if(receivedPackets.size() != 0){
		r_buffer_size = lastPacket->size;
		*r_buffer = (const uint8_t *)(&lastPacket->data);
		// r_buffer = lastPacket->data;
		// r_buffer = a.alloc.mem;
		receivedPackets.pop_front();
		return OK;
	}
	return Error::ERR_DOES_NOT_EXIST;
};

Error SteamNetworkPeer::put_packet(const uint8_t *p_buffer, int p_buffer_size) {
	// String a;
	// a.copy_from_unchecked(p_buffer,p_buffer_size);
	// steam->sendLobbyChatMsg();
	bool sentValid = SteamMatchmaking()->SendLobbyChatMsg(lobbyId, p_buffer, p_buffer_size);
	if(sentValid){
		return Error::OK;
	}
	else{
		ERR_PRINT("packed failed to send!");
		return Error::ERR_CONNECTION_ERROR;
	}
	// WARN_PRINT("not yet implemented!");
	// return Error::ERR_PRINTER_ON_FIRE;
};

int SteamNetworkPeer::get_max_packet_size() const {
	WARN_PRINT("not yet implemented!");
	return -1;
};

int SteamNetworkPeer::get_available_packet_count() const {
	return this->receivedPackets.size();
};


/* Specific to NetworkedMultiplayerPeer */
void SteamNetworkPeer::set_transfer_mode(SteamNetworkPeer::TransferMode p_mode) {
	this->transferMode = p_mode;
	TODO_PRINT("todo set the steam thing for these!");
};

SteamNetworkPeer::TransferMode SteamNetworkPeer::get_transfer_mode() const {
	return this->transferMode;
};

void SteamNetworkPeer::set_target_peer(int p_peer_id) {
	targetPeer = p_peer_id;
	TODO_PRINT("todo, prequalify steam id!");
};

int SteamNetworkPeer::get_packet_peer() const {
	return receivedPackets.front()->get()->sender.GetAccountID();
	if( isServer ){
		return 1;
	}
};

bool SteamNetworkPeer::is_server() const {
	return isServer;
};

void SteamNetworkPeer::poll() {
	//don't like this, but it's the default behaviour
	// Steam::get_singleton()->run_callbacks();
};

int SteamNetworkPeer::get_unique_id() const {
	return SteamUser()->GetSteamID().GetAccountID();
	// return receivedPackets.front()->get()->sender.GetAccountID();
	if(isServer){
		return 1;
	}
	CSteamID steam_id = SteamUser()->GetSteamID();
	WARN_PRINT("not yet implemented!");
	return -1;
};

void SteamNetworkPeer::set_refuse_new_connections(bool p_enable) {
	refuseConnections = p_enable;
	TODO_PRINT("figure out if I need to do something on the steam side here");
};

bool SteamNetworkPeer::is_refusing_new_connections() const {
	return refuseConnections;
};

SteamNetworkPeer::ConnectionStatus SteamNetworkPeer::get_connection_status() const {
	return connectionStatus;
};



/* Callbacks */


void SteamNetworkPeer::lobbyChatUpdate( uint64_t lobbyId, uint64_t changedId, uint64_t makingChangeId, uint32 chatState){
	WARN_PRINT("not yet implemented!");
};

void SteamNetworkPeer::lobbyCreated( int connect, uint64_t lobbyId){
	if( connect == 1 ){
		this->lobbyId = CSteamID(uint64(lobbyId));
		connectionStatus = ConnectionStatus::CONNECTION_CONNECTED;
		isServer = true;
		// emit_signal("connection_succeeded");
		// emit_signal("peer_connected",1);
	}
	else{
		ERR_PRINT("ERROR ON LOBBY CREATION!");
		emit_signal("connection_failed");
	}
};

void SteamNetworkPeer::lobbyDataUpdate( uint8 success, uint64_t lobbyId, uint64_t memberId){
	if(this->lobbyId.ConvertToUint64() != lobbyId){
		return;
	}
	else if(success == 1){
		if(lobbyId == memberId){
			lobbyData = steam->getAllLobbyData(lobbyId);
			// WARN_PRINT("lobbyGameCreated: todo, update lobby itself!");
		}
		else{
			TODO_PRINT("todo, user data!");
		}
	}
	else 
		ERR_PRINT("failed!");
};

void SteamNetworkPeer::lobbyJoined( uint64_t lobbyId, uint32_t permissions, bool locked, uint32_t response){
	if(response != k_EChatRoomEnterResponseSuccess){
		ERR_PRINT("Joined lobby failed!");
		emit_signal("connection_failed");
		return;
	}
	if(isServer){
		// emit_signal("peer_connected",1);
		TODO_PRINT("This signal shouldn't happen, but it was useful to test peer messages.");
		//don't do stuff if you're already the host
	}
	else{
		this->lobbyId = CSteamID(uint64(lobbyId));
		connectionStatus = ConnectionStatus::CONNECTION_CONNECTED;
		emit_signal("connection_succeeded");
	}
};

void SteamNetworkPeer::lobbyGameCreated( uint64_t lobbyId, uint64_t serverId, String serverIp, uint16 port){
	WARN_PRINT("not yet implemented!");
};

void SteamNetworkPeer::lobbyInvite( uint64_t inviter, uint64_t lobbyId, uint64_t game){
	WARN_PRINT("not yet implemented!");
};

void SteamNetworkPeer::lobbyMatchList( Array lobbies){
	WARN_PRINT("not yet implemented!");
};

void SteamNetworkPeer::lobbyKicked( uint64_t lobbyId, uint64_t adminId, uint8 dueToDisconnect){
	WARN_PRINT("not yet implemented!");
};


/* Steam_API direct callbacks */
void SteamNetworkPeer::lobbyMessage(LobbyChatMsg_t* call_data){
	if(lobbyId != call_data->m_ulSteamIDLobby){
		return;
	}
	Packet* packet = new Packet;

	packet->sender = call_data->m_ulSteamIDUser;
	if(SteamUser()->GetSteamID() == packet->sender){
		return;
	}
	uint8 chat_type = call_data->m_eChatEntryType;
	// Convert the chat type over
	EChatEntryType type = (EChatEntryType)chat_type;
	// Get the chat message data
	packet->size = SteamMatchmaking()->GetLobbyChatEntry(lobbyId, call_data->m_iChatID, &(packet->sender), &(packet->data), MAX_STEAM_PACKET_SIZE, &type);
	packet->channel = -1;

	receivedPackets.push_back(packet);

// 	lobbyMessage( lobby, user, out, chat_type);
// }
// void SteamNetworkPeer::lobbyMessage( uint64_t lobbyId, uint64_t user, PoolByteArray message, uint8 chatType){
	// p.channel
	// WARN_PRINT("not yet implemented!");
	// TODO_PRINT("here I need to store messages. Packets are consumed with get_packet calls!");
};
