OnPlayerConnectedSystem = System()
OnPlayerConnectedSystem.NumPlayers = 0
OnPlayerConnectedSystem.NumSpectators = 0
OnPlayerConnectedSystem.MaxPlayers = 5
OnPlayerConnectedSystem.PlayerId = 1


OnPlayerConnectedSystem.Update = function(self, dt)

end

OnPlayerConnectedSystem.Initialize = function(self)
	self:SetName("OnPlayerConnectedSystemSystem")
	self:InitializeNetworkEvents()
	
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	
	print("OnPlayerConnectedSystem initialized!")
end

OnPlayerConnectedSystem.PostInitialize = function(self)

	self:AddConnectedPlayers()

	print("OnPlayerConnectedSystem post initialized!")
end

OnPlayerConnectedSystem.OnEntityAdded = function(self, entityId)
	--world:SetComponent(entityId, "NetConnection", "Active", true);

end

OnPlayerConnectedSystem.OnEntityRemoved = function(self, entityId)
	--world:SetComponent(entityId, "NetConnection", "Active", false);
end

OnPlayerConnectedSystem.OnPlayerConnected = function(self, _ip, _port, _message)

	if #self:GetEntities("GameRunning") > 0 then

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
			--Net.Kick(_ip, _port, "Game has started.")
		end
		return
	end

	if self.NumPlayers >= self.MaxPlayers then
		
		if self.NumSpectators >= 5 then
			Net.Kick(_ip, _port, "Server is full.")
		else
			print("New spectator connected")
			self.NumSpectators = self.NumSpectators + 1
		end
		
		return
	end
	self.NumPlayers = self.NumPlayers + 1
	--	Hax new ID
	
	--	Create the new player
	local newName = "Player_" .. tostring(self.PlayerId)
	
	local newEntityId = world:CreateNewEntity("Player")

	print("Player_: " .. newEntityId)
	
	world:SetComponent(newEntityId, "PlayerName", "Name", newName);
	world:SetComponent(newEntityId, "NetConnection", "IpAddress", _ip);
	world:SetComponent(newEntityId, "NetConnection", "Port", _port);

	world:CreateComponentAndAddTo("ActiveNetConnection", newEntityId)

	self.PlayerId = self.PlayerId + 1
end

OnPlayerConnectedSystem.OnPlayerDisconnected = function(self, _ip, _port, _message)

	local entities = self:GetEntities();
	local foundPlayer = false
	
	
	for i = 1, #entities do
		
		local ip = self:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString()
		local port = self:GetComponent(entities[i], "NetConnection", "Port"):GetInt()
		
		if _ip == ip and _port == port then
			foundPlayer = true
			if #self:GetEntities("GameRunning") > 0 then
				world:RemoveComponentFrom("ActiveNetConnection", entities[i])
			else
				world:KillEntity(entities[i])		
			end
			break

		end
	end	
	
	if foundPlayer then
		self.NumPlayers = self.NumPlayers - 1
		
		if self.NumPlayers == 0 then
			Console.AddToCommandQueue("reload") -- Reload the gamemode to allow new players to connect
		end
		
	else
		print("Spectator disconnected")
		self.NumSpectators = self.NumSpectators - 1
	end


	
end

OnPlayerConnectedSystem.AddConnectedPlayers = function(self)
	
	local clients = { Net.ConnectedClients() }
	

	for i = 1, #clients, 2 do
		
		local ip = clients[i]
		local port = clients[i+1]

		if self.NumPlayers >= self.MaxPlayers then
			Net.Kick(ip, port, "Server is full.")
			return
		end
		self.NumPlayers = self.NumPlayers + 1
		--	Hax new ID
	
		--	Create the new player
		local newName = "Player_" .. tostring(self.PlayerId)
	
		local newEntityId = world:CreateNewEntity("Player")
	
		world:SetComponent(newEntityId, "PlayerName", "Name", newName);
		world:SetComponent(newEntityId, "NetConnection", "IpAddress", ip);
		world:SetComponent(newEntityId, "NetConnection", "Port", port);

		world:CreateComponentAndAddTo("ActiveNetConnection", newEntityId)
	
		--world:SetComponent(newEntityId, "PlayerNumber", "Number", self.ConnectedPlayers);

		self.PlayerId = self.PlayerId + 1

	end

end



























