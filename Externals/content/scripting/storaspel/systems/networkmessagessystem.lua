networkMessagesSystem = System()

networkMessagesSystem.Update = function(self, dt)

	Net.Update(dt)
	
	--if Server.IsRunning() then
	--	while Server.HandlePacket() > 0 do	end
	--end
		
	--if Client.IsConnected() then
	--	while Client.HandlePacket() > 0 do	end
	--end

end
networkMessagesSystem.Initialize = function(self)
	self:SetName("Network Message System")
	self:InitializeNetworkEvents()
	
	self:AddComponentTypeToFilter("SyncNetwork", FilterType.Mandatory)
	print("NetworkMessagesSystem initialized!")
end
networkMessagesSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
end
networkMessagesSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved (LUA)")
end

networkMessagesSystem.OnBannedFromServer = function(self, _ip, _port, _message)
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end

	Net.ResetNetworkMaps()
	
	local s = "[Client] You was banned from the server " .. _ip .. ":" .. _port .. ". Reason: " .. _message
	Console.Print(s)
end
networkMessagesSystem.OnConnectedToServer = function(self, _ip, _port)
	local id = Net.StartPack("Username")
	Net.WriteString(id, "Username_Lua")
	Net.SendToServer(id)
	local s = "[Client] fgfghkdfgbdkfgjdojhkljfgjhkldfgklö Connected to server " .. _ip .. ":" .. _port
	Console.Print(s)
end
networkMessagesSystem.OnDisconnectedFromServer = function(self, _ip, _port)
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	Net.ResetNetworkMaps()

	local s = "[Client] Disconnected from server " .. _ip .. ":" .. _port
	Console.Print(s)
end
networkMessagesSystem.OnFailedToConnect = function(self, _ip, _port)
	local s = "[Client] Failed to connect to server " .. _ip .. ":" .. _port
	Console.Print(s)	
end
networkMessagesSystem.OnKickedFromServer = function(self, _ip, _port, _message)
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	Net.ResetNetworkMaps()

	local s = "[Client] Kicked from server " .. _ip .. ":" .. _port .. ". Reason: " .. _message
	Console.Print(s)	
end
networkMessagesSystem.OnPasswordInvalid = function(self, _ip, _port)
	local s = "[Client] Invalid password to server " .. _ip .. ":" .. _port
	Console.Print(s)	
end
networkMessagesSystem.OnRemotePlayerBanned = function(self, _message)
	local s = "[Client] Remote player " .. _message .. " was banned from the server"
	Console.Print(s)
end
networkMessagesSystem.OnRemotePlayerConnected = function(self, _message)
	local s = "[Client] Remote player " .. _message .. " connected"
	Console.Print(s)
end
networkMessagesSystem.OnRemotePlayerDisconnected = function(self, _message)
	local s = "[Client] Remote player " .. _message .. " disconnected"
	Console.Print(s)
end
networkMessagesSystem.OnRemotePlayerKicked = function(self, _message)
	local s = "[Client] Remote player " .. _message .. " was kicked from the server"
	Console.Print(s)
end
networkMessagesSystem.OnServerFull = function(self, _ip, _port)
	local s = "[Client] Failed to connect to server " .. _ip .. ":" .. _port .. ". Server full"
	Console.Print(s)	
end
networkMessagesSystem.OnTimedOutFromServer = function(self, _ip, _port)
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	Net.ResetNetworkMaps()
	
	local s = "[Client] Timed out from server " .. _ip .. ":" .. _port
	Console.Print(s)	
end

networkMessagesSystem.OnPlayerConnected = function(self, _ip, _port, _message)
	
	-- Hämta alla entiteter som har komponenten "SyncNetwork"
	-- Skicka dessa entiteter till klienten
	
	local entities = self:GetEntities();
	
	for i = 1, #entities do
		Net.SendEntity(entities[i], _ip, _port)	
		Console.Print("Send Entity: " .. entities[i])
	end	
	
	local s = "[Server] Client " .. _ip .. ":" .. _port .. " connected to server"
	Console.Print(s)
end
networkMessagesSystem.OnPlayerDisconnected = function(self, _ip, _port)
local s = "[Server] Client " .. _ip .. ":" .. _port .. " disconnected from server"
	Console.Print(s)
end
networkMessagesSystem.OnPlayerTimedOut = function(self, _ip, _port)
local s = "[Server] Client " .. _ip .. ":" .. _port .. " timed out from the server"
	Console.Print(s)
end