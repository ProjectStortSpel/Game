SendPlayerInformation = System()

SendPlayerInformation.Initialize = function(self)
	self:SetName("SendPlayerInformation")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
end

SendPlayerInformation.EntitiesAdded = function(self, dt, entities)

	for n = 1, #entities do
		
		if world:EntityHasComponent(entities[n], "GameRunning") then
		
			local	allPlayers	=	self:GetEntities("Player")
			for i = 1, #allPlayers do
				self:SendGameStartedInformation(allPlayers[i])
			end
		end
		
		
	end
	
end

SendPlayerInformation.SendGameStartedInformation = function(self, playerId)

	local	unitEntityId	=	world:GetComponent(playerId, "UnitEntityId", "Id"):GetInt()
	
	--	Get connection information
	local	IP		= 	world:GetComponent(playerId, "NetConnection", "IpAddress"):GetText()
	local	PORT	= 	world:GetComponent(playerId, "NetConnection", "Port"):GetInt()
	
	local	id	=	Net.StartPack("Client.GetPlayerEntityId")
					Net.WriteInt(id, Net.ToClientID(unitEntityId))
					Net.Send(id, IP, PORT)
end