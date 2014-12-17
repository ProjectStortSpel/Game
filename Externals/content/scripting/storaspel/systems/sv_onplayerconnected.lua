OnPlayerConnectedSystem = System()
OnPlayerConnectedSystem.ConnectedPlayers = 0


OnPlayerConnectedSystem.Update = function(self, dt)

end

OnPlayerConnectedSystem.Initialize = function(self)
	self:SetName("On Player ConnectedSystem System")
	self:InitializeNetworkEvents()
	
	self:AddComponentTypeToFilter("Player", FilterType.Mandatory)
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory)
	
	print("On Player Connected System!")
end

OnPlayerConnectedSystem.OnEntityAdded = function(self, entityId)
	world:SetComponent(entityId, "NetConnection", "Active", true);
end

OnPlayerConnectedSystem.OnEntityRemoved = function(self, entityId)
	world:SetComponent(entityId, "NetConnection", "Active", false);
end

OnPlayerConnectedSystem.OnPlayerConnected = function(self, _ip, _port, _message)

	--	Hax new ID
	self.ConnectedPlayers = self.ConnectedPlayers + 1
	
	--	Create the new player
	local newName = "Player_" .. tostring(self.ConnectedPlayers)
	
	local newEntityId = world:CreateNewEntity("Player")
	local strAdress = _ip .. ""
	local strPort = _port .. ""
	
	world:SetComponent(newEntityId, "Player", "PlayerName", newName);
	world:SetComponent(newEntityId, "NetConnection", "IpAddress", strAdress);
	world:SetComponent(newEntityId, "NetConnection", "Port", strPort);
	world:SetComponent(newEntityId, "NetConnection", "Active", true);
	
	world:SetComponent(newEntityId, "Player", "PlayerNumber", self.ConnectedPlayers);
end





























