OnPlayerConnectedSystem = System()
OnPlayerConnectedSystem.NumPlayers = 0
OnPlayerConnectedSystem.PlayerId = 1


OnPlayerConnectedSystem.Update = function(self, dt)

end

OnPlayerConnectedSystem.Initialize = function(self)
	self:SetName("On Player ConnectedSystem System")
	self:InitializeNetworkEvents()
	
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	
	print("On Player Connected System!")
end

OnPlayerConnectedSystem.OnEntityAdded = function(self, entityId)
	--world:SetComponent(entityId, "NetConnection", "Active", true);

end

OnPlayerConnectedSystem.OnEntityRemoved = function(self, entityId)
	--world:SetComponent(entityId, "NetConnection", "Active", false);
end

OnPlayerConnectedSystem.OnPlayerConnected = function(self, _ip, _port, _message)

	if #self.GetEntities("GameRunning") > 0 then

		local foundPlayer = false
		local entities = self:GetEntities();		
		for i = 1, #entities do
		
			local ip = self:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString()
			local port = self:GetComponent(entities[i], "NetConnection", "Port"):GetInt()
		
			if _ip == ip and _port == port then
				world:CreateComponentAndAddTo("ActiveNetConnection", entities[i])				
				break
			end
		end	
		
		if not foundPlayer then
			Net.Kick(_ip, _port, "Game has started.")
		end
		return
	end

	if self.NumPlayers >= 2 then
		
		Net.Kick(_ip, _port, "Server is full.")
		return
	end
	self.NumPlayers = self.NumPlayers + 1
	--	Hax new ID
	
	--	Create the new player
	local newName = "Player_" .. tostring(self.PlayerId)
	
	local newEntityId = world:CreateNewEntity("Player")
	local strAdress = _ip .. ""
	local strPort = _port .. ""
	
	world:SetComponent(newEntityId, "PlayerName", "Name", newName);
	world:SetComponent(newEntityId, "NetConnection", "IpAddress", strAdress);
	world:SetComponent(newEntityId, "NetConnection", "Port", strPort);

	world:CreateComponentAndAddTo("ActiveNetConnection", newEntityId)
	
	--world:SetComponent(newEntityId, "PlayerNumber", "Number", self.ConnectedPlayers);

	self.PlayerId = self.PlayerId + 1
end

OnPlayerConnectedSystem.OnPlayerDisconnected = function(self, _ip, _port, _message)

	local entities = self:GetEntities();
	
	for i = 1, #entities do
		
		local ip = self:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString()
		local port = self:GetComponent(entities[i], "NetConnection", "Port"):GetInt()
		
		if _ip == ip and _port == port then

			if #self.GetEntities("GameRunning") > 0 then
				world:RemoveComponentFrom("ActiveNetConnection", entities[i])
			else
				world:KillEntity(entities[i])		
			end
			break

		end
	end	
	self.NumPlayers = self.NumPlayers - 1		
end





























