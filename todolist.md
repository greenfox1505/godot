# TODO LIST


## Singals

- [x] `network_peer_connected`
- [ ] `network_peer_disconnected`
- [x] `connected_to_server`
- [ ] `connection_failed`
- [ ] `server_disconnected`

## Functions

### Node

- [x] `rpc("function_name", <optional_args>)`
- [ ] `rpc_id(<peer_id>,"function_name", <optional_args>)`
- [x] `rpc_unreliable("function_name", <optional_args>)`
- [ ] `rpc_unreliable_id(<peer_id>, "function_name", <optional_args>)`

### get_tree()

- [ ] `get_rpc_sender_id()`
- [ ] `get_network_unique_id()`
- [ ] `get_tree().network_peer = steam_network_peer`
  - [ ] `get_network_peer()`
  - [ ] `set_network_peer()`
- [ ] `is_network_server`


### Internal Systems

- [ ] Translate all GodotIDs to SteamIDs.
- [ ] Translate all SteamIDs to GodotIDs.
- [ ] Verify Disconnect 

### CodeFlow

ENet Code Flow:
```md
# Host
- create network peer as host
- connect to tree
# Client
- create network peer as client
- connect to ip address
- connect to tree
```

Steam Code Flow:
```md
# Host
- create network peer as host
- connect to tree
# Client
- detect join signal
- create network peer as client
- connect to ip address
- connect to tree
```

Every API should run cleanup before running