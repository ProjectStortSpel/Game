TakeCardsFromPlayerSystem = System()

TakeCardsFromPlayerSystem.Initialize = function ( self )
	self:SetName("TakeCardsFromPlayerSystem")
	self:AddComponentTypeToFilter("TakeCardsFromPlayer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealtCard", FilterType.RequiresOneOf)
end

TakeCardsFromPlayerSystem.OnEntityAdded = function(self, entityId)
		
	if world:EntityHasComponent( entityId, "TakeCardsFromPlayer") then
		
		local player = world:GetComponent(entityId, "TakeCardsFromPlayer", "Player"):GetInt()
		local cards = self:GetEntities("DealtCard")

		for i = 1, #cards do
			
			local owner = world:GetComponent(cards[i], "DealtCard", "PlayerEntityId"):GetInt()
			
			if owner == player then
				
				world:RemoveComponentFrom("DealtCard", cards[i])

				local ip = world:GetComponent(player, "NetConnection", "IpAddress"):GetString()
				local port = world:GetComponent(player, "NetConnection", "Port"):GetInt()

				Net.SendEntityKill(cards[i], ip, port)

			end

		end

		world:KillEntity( entityId )

	end
	
end
