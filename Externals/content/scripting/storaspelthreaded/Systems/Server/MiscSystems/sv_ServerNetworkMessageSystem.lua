ServerNetworkMessageSystem = System()

ServerNetworkMessageSystem.Initialize = function(self)
	
	--	Set Name
	self:SetName("ServerNetworkMessageSystem")
	
	self:UsingEntitiesAdded()
	
	self:InitializeNetworkEvents()
	
	
	--	Set Filter
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerNameChanged", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("SyncNetwork", FilterType.RequiresOneOf)
end

ServerNetworkMessageSystem.PostInitialize = function(self)
	
	local playerCounter = world:CreateNewEntity()
	world:CreateComponentAndAddTo("PlayerCounter", playerCounter)

	world:SetComponent(playerCounter, "PlayerCounter", "AIs", 0)
	world:SetComponent(playerCounter, "PlayerCounter", "Players", 0)
	world:SetComponent(playerCounter, "PlayerCounter", "Spectators", 0)
end

ServerNetworkMessageSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]
		
		if world:EntityHasComponent(entity, "PlayerNameChanged") then
		
			local players 	= self:GetEntities("Player")
			local pnIp 		= world:GetComponent(entity, "PlayerNameChanged", "IpAddress"):GetText()
			local pnPort 	= world:GetComponent(entity, "PlayerNameChanged", "Port"):GetInt()
			local pnName 	= world:GetComponent(entity, "PlayerNameChanged", "Name"):GetString()
		
			for i = 1, #players do
				
				local ip 	= world:GetComponent(players[i], "NetConnection", "IpAddress"):GetText()
				local port 	= world:GetComponent(players[i], "NetConnection", "Port"):GetInt()
				
				if pnIp == ip and pnPort == port then
					world:GetComponent(players[i], "PlayerName", "Name"):SetString(pnName)
					world:KillEntity(entity)
					return
				end
				
			end
		
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

ServerNetworkMessageSystem.AddPlayer = function(self, _ip, _port, _spectator)

	local player = world:CreateNewEntity("Player")
	world:SetComponent(player, "NetConnection", "IpAddress", _ip)
	world:SetComponent(player, "NetConnection", "Port", _port)
	world:CreateComponentAndAddTo("ActiveNetConnection", player)

end

ServerNetworkMessageSystem.OnPlayerConnected = function(self, _ip, _port, _message)

	local addSpectator = false
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", 0)
	local noOfAIs, noOfPlayers, noOfSpectators = counterComp:GetInt3()

	local mapSpecs = self:GetEntities("MapSpecs")
	local maxPlayers = 1
	if #mapSpecs > 0 then
		maxPlayers = world:GetComponent(mapSpecs[1], "MapSpecs", "NoOfSpawnpoints"):GetInt(0)
	end
	
	if #self:GetEntities("GameRunning") > 0 then -- If the game is running
		
		local playerFound = false
		local entities = self:GetEntities("Player")
		
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
	
	local name = Net.GetPlayerName(_ip, _port)
	world:GetComponent(newPlayer, "PlayerName", "Name"):SetString(name)
	print(name .. " connected")
	
	if addSpectator then
		world:CreateComponentAndAddTo("IsSpectator", newPlayer)
		self:CounterComponentChanged(1, "Spectators")
	else
		self:CounterComponentChanged(1, "Players")
	end
	
	local sync = self:GetEntities("SyncNetwork");
	
	for i = 1, #sync do
		Net.SendEntity(sync[i], _ip, _port)	
		--Console.Print("Send Entity: " .. sync[i])
	end	
	
	Net.Send(Net.StartPack("RemoveLoadingScreen"), _ip, _port)	

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
				world:KillEntity(entities[i])
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
				world:KillEntity(entities[i])
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

Net.Receive("CLIENT_SET_NAME", 
	function(id, ip, port)
		
		local name = Net.ReadString(id)
		name = Net.SetPlayerName(ip, port, name)
		
		local newEntity = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PlayerNameChanged", newEntity)
		world:GetComponent(newEntity, "PlayerNameChanged", "Name"):SetString(name)
		world:SetComponent(newEntity, "PlayerNameChanged", "IpAddress", ip)
		world:SetComponent(newEntity, "PlayerNameChanged", "Port", port)
		
	end 
)
