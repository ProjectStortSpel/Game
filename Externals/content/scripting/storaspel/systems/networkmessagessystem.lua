networkMessagesSystem = System()

networkMessagesSystem.Update = function(self, dt)
end
networkMessagesSystem.Initialize = function(self)
	self:InitializeNetworkEvents()

	self:AddComponentTypeToFilter("Network", FilterType.Mandatory)
	print("NetworkMessagesSystem initialized!")
end
networkMessagesSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
end
networkMessagesSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved (LUA)")
end

networkMessagesSystem.OnBannedFromServer = function(self, _ip, _port, _message)
	print("[Client] Banned from server " .. _ip .. ":" .. _port)
end
networkMessagesSystem.OnConnectedToServer = function(self, _ip, _port)
	print("[Client] Connected to server " .. _ip .. ":" .. _port)
end
networkMessagesSystem.OnDisconnectedFromServer = function(self, _ip, _port)
	print("[Client] Disconnected from server " .. _ip .. ":" .. _port)
end
networkMessagesSystem.OnFailedToConnect = function(self, _ip, _port)
	print("[Client] Failed to connect to server " .. _ip .. ":" .. _port)
end
networkMessagesSystem.OnKickedFromServer = function(self, _ip, _port, _message)
	print("[Client] Kicked from server " .. _ip .. ":" .. _port)
end
networkMessagesSystem.OnPasswordInvalid = function(self, _ip, _port)
	print("[Client] Invalid password to server " .. _ip .. ":" .. _port)
end
networkMessagesSystem.OnRemotePlayerBanned = function(self, _ip, _port, _message)
	print("[Client] Remote player banned from server")
end
networkMessagesSystem.OnRemotePlayerConnected = function(self, _ip, _port, _message)
	print("[Client] Remote player connected")
	print(_message)
end
networkMessagesSystem.OnRemotePlayerDisconnected = function(self, _ip, _port, _message)
	print("[Client] Remote player disconnected")
end
networkMessagesSystem.OnRemotePlayerKicked = function(self, _ip, _port, _message)
	print("[Client] Remote player kicked")
end
networkMessagesSystem.OnServerFull = function(self, _ip, _port)
	print("[Client] Server full")
end
networkMessagesSystem.OnTimedOutFromServer = function(self, _ip, _port)
	print("[Client] Timed out from server")
end

networkMessagesSystem.OnPlayerConnected = function(self, _ip, _port)
	print("[Server] Client " .. _ip .. ":" .. _port .. " connected to server")
end
networkMessagesSystem.OnPlayerDisconnected = function(self, _ip, _port)
	print("[Server] Client " .. _ip .. ":" .. _port .. " disconnected from server")
end
networkMessagesSystem.OnPlayerTimedOut = function(self, _ip, _port)
	print("[Server] Client " .. _ip .. ":" .. _port .. " timed out from server")
end