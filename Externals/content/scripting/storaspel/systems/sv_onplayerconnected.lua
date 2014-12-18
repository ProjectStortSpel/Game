OnPlayerConnectedSystem = System()
OnPlayerConnectedSystem.NumPlayers = 0
OnPlayerConnectedSystem.PlayerId = 1


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

	if GameRunning then

		local entities = self:GetEntities();


		for i = 1, #entities do
		
			local ip = self:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString()
			local port = self:GetComponent(entities[i], "NetConnection", "Port"):GetInt()
		
			if _ip == ip and _port == port then

				if GameRunning then
					world:RemoveComponentFrom("ActiveNetConnection", entities[i])
				else
					world:KillEntity(entities[i])
					self.NumPlayer = self.NumPlayer - 1				
				end
			
				break

			end
		end	

		
		--Kolla om spelaren finns och låt han komma tbx
		Net.Kick(_ip, _port, "Game has started.")
		return
	end

	if self.NumPlayer > 4 then
		
		Net.Kick(_ip, _port, "Server is full.")
		return
	end
	self.NumPlayer = self.NumPlayer + 1
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

			if GameRunning then
				world:RemoveComponentFrom("ActiveNetConnection", entities[i])
			else
				world:KillEntity(entities[i])
				self.NumPlayer = self.NumPlayer - 1				
			end
			
			break

		end
	end	
end





























