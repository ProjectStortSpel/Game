ServerNetworkMessageSystem = System()

ServerNetworkMessageSystem.Initialize = function(self)
	
	--	Set Name
	self:SetName("ServerNetworkMessageSystem")
	
	self:InitializeNetworkEvents()
	
	--	Set Filter
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("SyncNetwork", FilterType.RequiresOneOf)
end

ServerNetworkMessageSystem.PostInitialize = function(self)
	
	local playerCounter = world:CreateNewEntity()
	world:CreateComponentAndAddTo("PlayerCounter", playerCounter)
	
	-- TODO: Change MaxPlayers based on the map loaded
	-- TODO: Note, moved to the Mapspecs-entity
	--world:SetComponent(playerCounter, "PlayerCounter", "MaxPlayers", 0)
	
	world:SetComponent(playerCounter, "PlayerCounter", "Players", 0)
	world:SetComponent(playerCounter, "PlayerCounter", "Spectators", 0)

	--self:AddConnectedPlayers(playerCounter, 9)
end

ServerNetworkMessageSystem.CounterComponentChanged = function(self, _change, _component)
	
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", _component)
	local number = counterComp:GetInt()
	number = number + _change
	world:SetComponent(counterEntities[1], "PlayerCounter", _component, number)
end

ServerNetworkMessageSystem.AddConnectedPlayers = function(self, _counterEntity, _maxPlayers)
	
	local clients = { Net.ConnectedClients() }
	clients.__mode = "k"
	local maxPlayers = _maxPlayers
	local noOfPlayers = 0

	for i = 1, #clients, 2 do
		
		local ip = clients[i]
		local port = clients[i+1]

		if noOfPlayers >= maxPlayers then
			Net.Kick(ip, port, "Server is full.")
			return
		end
		
		noOfPlayers = noOfPlayers + 1
	
		--	Create the new player
		local newName = "Player_" .. tostring(noOfPlayers + 1)
	
		local newEntityId = world:CreateNewEntity("Player")
	
		world:SetComponent(newEntityId, "PlayerName", "Name", newName);
		world:SetComponent(newEntityId, "NetConnection", "IpAddress", ip);
		world:SetComponent(newEntityId, "NetConnection", "Port", port);

		world:CreateComponentAndAddTo("ActiveNetConnection", newEntityId)
	
		--world:SetComponent(newEntityId, "PlayerNumber", "Number", self.ConnectedPlayers);

	end
	world:SetComponent(_counterEntity, "PlayerCounter", "Players", noOfPlayers)
end

ServerNetworkMessageSystem.OnPlayerConnected = function(self, _ip, _port, _message)

	local addSpectator = false
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", 0)
	local noOfPlayers, noOfSpectators = counterComp:GetInt2()
	
	local maxPlayers = world:GetComponent(self:GetEntities("MapSpecs")[1], "MapSpecs", "NoOfSpawnpoints"):GetInt(0)
	
	if #self:GetEntities("GameRunning") > 0 then -- If the game is running
		
		local playerFound = false
		local entities = self:GetEntities()
		
		for i = 1, #entities do -- Go through all entities
		
			local ip = world:GetComponent(entities[i], "NetConnection", "IpAddress"):GetText()
			local port = world:GetComponent(entities[i], "NetConnection", "Port"):GetInt()	

			if _ip == ip and _port == port then
				world:CreateComponentAndAddTo("ActiveNetConnection", entities[i])
				playerFound = true
				print("Player reconnected")
				return
			end
			
		end
	
		if not playerFound then
			addSpectator = true
			print("player not found")
		end
	
	else -- If the game is not running yet
		if noOfPlayers >= maxPlayers then
			addSpectator = true
		end
	
	end
	
	local newPlayer = world:CreateNewEntity("Player")
	
	world:SetComponent(newPlayer, "NetConnection", "IpAddress", _ip);
	world:SetComponent(newPlayer, "NetConnection", "Port", _port);
	world:CreateComponentAndAddTo("ActiveNetConnection", newPlayer)
	
	if addSpectator then
	
		local newName = "Spectator_" .. tostring(noOfSpectators + 1)
		world:SetComponent(newPlayer, "PlayerName", "Name", newName);
	
	
		world:CreateComponentAndAddTo("IsSpectator", newPlayer)
		self:CounterComponentChanged(1, "Spectators")
		
		print("Spectator: " .. newPlayer .. " connected")
	else
	
		local newName = "Player_" .. tostring(noOfPlayers + 1)
		world:SetComponent(newPlayer, "PlayerName", "Name", newName);
		
		self:CounterComponentChanged(1, "Players")
		
		print("Player: " .. newPlayer .. " connected")
	end
	
	local sync = self:GetEntities("SyncNetwork");
	
	for i = 1, #sync do
		Net.SendEntity(sync[i], _ip, _port)	
		Console.Print("Send Entity: " .. sync[i])
	end	

end

ServerNetworkMessageSystem.OnPlayerDisconnected = function(self, _ip, _port, _message)

	local entities = self:GetEntities("Player");
	local foundPlayer = false
	local isSpectator = false
	
	for i = 1, #entities do
		
		local ip = world:GetComponent(entities[i], "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(entities[i], "NetConnection", "Port"):GetInt()
		
		if _ip == ip and _port == port then
			foundPlayer = true
			if #self:GetEntities("GameRunning") > 0 then
				world:RemoveComponentFrom("ActiveNetConnection", entities[i])
			else
				world:KillEntity(entities[i])		
			end
			
			isSpectator = world:EntityHasComponent(entities[i], "IsSpectator")
			
			break

		end
	end	
	
	if foundPlayer then
		
		if isSpectator then
			print("Spectator disconnected")
			self:CounterComponentChanged(-1, "Spectators")
		else
			print("Player disconnected")
			self:CounterComponentChanged(-1, "Players")
		end
	end
	
end

ServerNetworkMessageSystem.OnPlayerTimedOut = function(self, _ip, _port, _message)

	local entities = self:GetEntities("Player");
	local foundPlayer = false
	local isSpectator = false
	
	for i = 1, #entities do
		
		local ip = world:GetComponent(entities[i], "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(entities[i], "NetConnection", "Port"):GetInt()
		
		if _ip == ip and _port == port then
			foundPlayer = true
			if #self:GetEntities("GameRunning") > 0 then
				world:RemoveComponentFrom("ActiveNetConnection", entities[i])
			else
				world:KillEntity(entities[i])		
			end
			
			isSpectator = world:EntityHasComponent(entities[i], "IsSpectator")
			
			break

		end
	end	
	
	if foundPlayer then
		
		if isSpectator then
			print("Spectator timed out")
			self:CounterComponentChanged(-1, "Spectators")
		else
			print("Player timed out")
			self:CounterComponentChanged(-1, "Players")
		end
	end
	
end

ServerNetworkMessageSystem.OnPasswordInvalid = function(self, _ip, _port, _message)
	print("ServerNetworkMessageSystem.OnPasswordInvalid - Not implemented!")
end

ServerNetworkMessageSystem.EntitiesAdded = function(self, dt, _entities)
	
	for i = 1, #_entities do 
		if world:EntityHasComponent( _entities[i], "MapSpecs") then
			
			local mapSpecsComp = world:GetComponent(self:GetEntities("MapSpecs")[1], "MapSpecs", "NoOfSpawnpoints")
			local noOfSpawnPoints = mapSpecsComp:GetInt()
			local counterEntities = self:GetEntities("PlayerCounter")
			self:AddConnectedPlayers(counterEntities[1], noOfSpawnPoints)
		end
	end
end