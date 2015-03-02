ServerNetworkMessageSystem = System()

ServerNetworkMessageSystem.Initialize = function(self)
	
	--	Set Name
	self:SetName("ServerNetworkMessageSystem")
	self:UsingEntitiesAdded()
	self:InitializeNetworkEvents()
	
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("SyncNetwork", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	
end

ServerNetworkMessageSystem.PostInitialize = function(self)
	
	local playerCounter = world:CreateNewEntity()
	world:CreateComponentAndAddTo("PlayerCounter", playerCounter)

	world:SetComponent(playerCounter, "PlayerCounter", "Players", 0)
	world:SetComponent(playerCounter, "PlayerCounter", "Spectators", 0)
end


ServerNetworkMessageSystem.EntitiesAdded = function(self, dt, _entities)

	for i = 1, #_entities do 
		local entity = _entities[i]
		if world:EntityHasComponent(entity, "MapSpecs") then
		end
	end
end

ServerNetworkMessageSystem.CounterComponentChanged = function(self, _change, _component)
	
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", _component)
	local number = counterComp:GetInt()
	number = number + _change
	world:SetComponent(counterEntities[1], "PlayerCounter", _component, number)
end

ServerNetworkMessageSystem.AddPlayer = function(self, _ip, _port, _playNo)
	local player = world:CreateNewEntity("Player")
	world:SetComponent(player, "NetConnection", "IpAddress", _ip)
	world:SetComponent(player, "NetConnection", "Port", _port)
	world:CreateComponentAndAddTo("ActiveNetConnection", player)
	
	local newName = "Player_" .. tostring(_playNo)
	world:SetComponent(player, "PlayerName", "Name", newName);
	self:CounterComponentChanged(1, "Players")
	
	print("Player: " .. newName .. " connected.")

end

ServerNetworkMessageSystem.AddSpectator = function(self, _ip, _port, _specNo)

	local spectator = world:CreateNewEntity("Player")
	world:SetComponent(spectator, "NetConnection", "IpAddress", _ip)
	world:SetComponent(spectator, "NetConnection", "Port", _port)
	world:CreateComponentAndAddTo("ActiveNetConnection", spectator)
	
	local newName = "Spectator_" .. tostring(_specNo)
	world:SetComponent(spectator, "PlayerName", "Name", newName);
	
	world:CreateComponentAndAddTo("IsSpectator", spectator)
	
	self:CounterComponentChanged(1, "Spectators")
	
	print("Spectator: " .. newName .. " connected.")

end

ServerNetworkMessageSystem.ReonnectPlayer = function(self)
	print("ServerNetworkMessageSystem.ReconnectPlayer is not yet implemented")
end

ServerNetworkMessageSystem.CheckPlayingState = function(self, _players, _ip, _port)

	local playerCounter = self:GetEntities("PlayerCounter")
	local noOfPlayers, noOfSpectators = world:GetComponent(playerCounter[1], "PlayerCounter", 0):GetInt2()

	local playerFound = false

	for i = 1, #_players do
		local ip 	= world:GetComponent(players[i], "NetConnection", "IpAddress"):GetText()
		local port 	= world:GetComponent(players[i], "NetConnection", "Port"):GetInt()
		
		if _ip == ip and _port == port then
			print("Player reconnected")
			playerFound = true
			break
		end
	end
	
	if playerFound then
		self:ReconnectPlayer()
	else
		self:AddSpectator(_ip, _port, noOfSpectators+1)
	end
	
	local sync = self:GetEntities("SyncNetwork");
	for i = 1, #sync do
		Net.SendEntity(sync[i], _ip, _port)	
		Console.Print("Send Entity: " .. sync[i])
	end	
	
	
end

ServerNetworkMessageSystem.CheckLobbyState = function(self, _players, _ip, _port)

	local playerCounter = self:GetEntities("PlayerCounter")
	local noOfPlayers, noOfSpectators = world:GetComponent(playerCounter[1], "PlayerCounter", 0):GetInt2()
	
	local maxPlayers = 1
	local mapSpecs = self:GetEntities("MapSpecs")
	if #mapSpecs > 0 then
		maxPlayers = world:GetComponent(mapSpecs[1], "MapSpecs", "NoOfSpawnpoints"):GetInt()
	end
	
	if noOfPlayers >= maxPlayers then
		self:AddSpectator(_ip, _port, noOfPlayers+1)
	else
		self:AddPlayer(_ip, _port, noOfSpectators+1)
	end
	
end

ServerNetworkMessageSystem.OnPlayerConnected = function(self, _ip, _port, _message)
	
	local players = self:GetEntities("Player")
    
	if #self:GetEntities("GameRunning") > 0 then
		self:CheckPlayingState(players, _ip, _port)
	else
		self:CheckLobbyState(players, _ip, _port)
	end
    --
	--
	--local mapSpecs = self:GetEntities("MapSpecs")
	--if #mapSpecs > 0 then
	--	maxPlayers = world:GetComponent(mapSpecs[1], "MapSpecs", "NoOfSpawnpoints"):GetInt()
	--end
	--
end

ServerNetworkMessageSystem.OnPlayerDisconnected = function(self, _ip, _port, _message)

	local players = self:GetEntities("Player")
	local foundPlayer = false
	
	for i = 1, #players do
		local ip = world:GetComponent(players[i], "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(players[i], "NetConnection", "Port"):GetInt()
		
		if _ip == ip and _port == port then
			foundPlayer = true
			isSpectator = world:EntityHasComponent(players[i], "IsSpectator")
			if #self:GetEntities("GameRunning") > 0 then
				--world:RemoveComponentFrom("ActiveNetConnection", players[i])
				world:KillEntity(players[i])
			else
				world:KillEntity(players[i])
			end
			break
		end
		
	end

	if foundPlayer then
	
		if isSpectator then
			print("Spectator disconnected.")
			self:CounterComponentChanged(-1, "Spectators")
		else
			print("Player disconnected.")
			self:CounterComponentChanged(-1, "Players")
		end
	end
	
end

ServerNetworkMessageSystem.OnPlayerTimedOut = function(self, _ip, _port, _message)

	local players = self:GetEntities("Player")
	local foundPlayer = false
	
	for i = 1, #players do
		local ip = world:GetComponent(players[i], "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(players[i], "NetConnection", "Port"):GetInt()
		
		if _ip == ip and _port == port then
			foundPlayer = true
			isSpectator = world:EntityHasComponent(players[i], "IsSpectator")
			if #self:GetEntities("GameRunning") > 0 then
				--world:RemoveComponentFrom("ActiveNetConnection", players[i])
				world:KillEntity(players[i])
			else
				world:KillEntity(players[i])
			end
			break
		end
		
	end

	if foundPlayer then
	
		if isSpectator then
			print("Spectator disconnected.")
			self:CounterComponentChanged(-1, "Spectators")
		else
			print("Player disconnected.")
			self:CounterComponentChanged(-1, "Players")
		end
	end
	
end

ServerNetworkMessageSystem.OnPasswordInvalid = function(self, _ip, _port, _message)
	print("ServerNetworkMessageSystem.OnPasswordInvalid - Not implemented!")
end