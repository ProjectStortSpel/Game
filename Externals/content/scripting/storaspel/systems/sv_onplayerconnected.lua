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
	--world:SetComponent(entityId, "NetConnection", "Active", true);
end

OnPlayerConnectedSystem.OnEntityRemoved = function(self, entityId)
	--world:SetComponent(entityId, "NetConnection", "Active", false);
end

OnPlayerConnectedSystem.OnPlayerConnected = function(self, _ip, _port, _message)

	--	Hax new ID
	self.ConnectedPlayers = self.ConnectedPlayers + 1
	
	--	Create the new player
	local newName = "Player_" .. tostring(self.ConnectedPlayers)
	
	local newEntityId = world:CreateNewEntity("Player")
	local strAdress = _ip .. ""
	local strPort = _port .. ""
	
	world:SetComponent(newEntityId, "PlayerName", "Name", newName);
	world:SetComponent(newEntityId, "NetConnection", "IpAddress", strAdress);
	world:SetComponent(newEntityId, "NetConnection", "Port", strPort);

	world:CreateComponentAndAddTo("ActiveNetConnection", newEntityId)
	
	--world:SetComponent(newEntityId, "PlayerNumber", "Number", self.ConnectedPlayers);
end

OnPlayerConnectedSystem.OnPlayerDisconnected = function(self, _ip, _port, _message)

	local entities = self:GetEntities();
	
	for i = 1, #entities do
		
		local ip = self:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString()
		local port = self:GetComponent(entities[i], "NetConnection", "Port"):GetInt()
		
		if _ip == ip and _port == port then

			world:RemoveComponentFrom("ActiveNetConnection", entities[i])
			break

		end
	end	
end





























