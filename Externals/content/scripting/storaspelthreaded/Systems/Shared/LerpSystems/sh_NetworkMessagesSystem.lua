NetworkMessagesSystem = System()

NetworkMessagesSystem.Initialize = function(self)
	self:SetName("Network Message System")
	--self:InitializeNetworkEvents()
	
	self:AddComponentTypeToFilter("SyncNetwork", FilterType.Mandatory)
end

NetworkMessagesSystem.OnBannedFromServer = function(self, _ip, _port, _message)
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end

	Net.ResetNetworkMaps()
end

NetworkMessagesSystem.OnDisconnectedFromServer = function(self, _ip, _port)
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	Net.ResetNetworkMaps()
end
NetworkMessagesSystem.OnFailedToConnect = function(self, _ip, _port)
	local s = "[Client] Failed to connect to server " .. _ip .. ":" .. _port
	Console.Print(s)	
end
NetworkMessagesSystem.OnKickedFromServer = function(self, _ip, _port, _message)
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	Net.ResetNetworkMaps()

	local s = "[Client] Kicked from server " .. _ip .. ":" .. _port .. ". Reason: " .. _message
	Console.Print(s)	
end

NetworkMessagesSystem.OnTimedOutFromServer = function(self, _ip, _port)
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	Net.ResetNetworkMaps()
end

NetworkMessagesSystem.OnPlayerConnected = function(self, _ip, _port, _message)
	-- Hämta alla entiteter som har komponenten "SyncNetwork"
	-- Skicka dessa entiteter till klienten
	
	local entities = self:GetEntities();
	
	for i = 1, #entities do
		Net.SendEntity(entities[i], _ip, _port)	
		Console.Print("Send Entity: " .. entities[i])
	end	
end
