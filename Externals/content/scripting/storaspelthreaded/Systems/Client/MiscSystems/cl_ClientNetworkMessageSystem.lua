ClientNetworkMessageSystem = System()

ClientNetworkMessageSystem.Initialize = function(self)
	
	--	Set Name
	self:SetName("ClientNetworkMessageSystem")
	
	self:InitializeNetworkEvents()
	
	--	Set Filter
	self:AddComponentTypeToFilter("SyncNetwork", FilterType.Mandatory)
end

ClientNetworkMessageSystem.OnConnectedToServer = function(self, _ip, _port, _message)
end

ClientNetworkMessageSystem.OnBannedFromServer = function(self, _ip, _port, _message)

	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end

	Net.ResetNetworkMaps()

end

ClientNetworkMessageSystem.OnDisconnectedFromServer = function(self, _ip, _port, _message)

	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	
	Net.ResetNetworkMaps()

end

ClientNetworkMessageSystem.OnFailedToConnect = function(self, _ip, _port, _message)
	local s = "Failed to connect to server " .. _ip .. ":" .. _port .. 
	Console.Print(s)
end

ClientNetworkMessageSystem.OnKickedFromServer = function(self, _ip, _port, _message)
	
	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	Net.ResetNetworkMaps()
	
end

ClientNetworkMessageSystem.OnServerFull = function(self, _ip, _port, _message)

	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	Net.ResetNetworkMaps()
	
end

ClientNetworkMessageSystem.OnTimedOutFromServer = function(self, _ip, _port, _message)

	local entities = self:GetEntities();
	for i = 1, #entities do
		world:KillEntity(entities[i])
	end
	Net.ResetNetworkMaps()
	
end


ClientNetworkMessageSystem.OnRemotePlayerConnected = function(self, _message)
end

ClientNetworkMessageSystem.OnRemotePlayerBanned = function(self, _message)
end

ClientNetworkMessageSystem.OnRemotePlayerDisconnected = function(self, _message)
end

ClientNetworkMessageSystem.OnRemotePlayerKicked = function(self, _message)
end