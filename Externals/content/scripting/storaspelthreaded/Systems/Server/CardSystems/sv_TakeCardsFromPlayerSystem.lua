TakeCardsFromPlayerSystem = System()

TakeCardsFromPlayerSystem.Initialize = function ( self )

	--	Set Name
	self:SetName("TakeCardsFromPlayerSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("TakeCardsFromPlayer", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealtCard", FilterType.RequiresOneOf)
end

TakeCardsFromPlayerSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, "TakeCardsFromPlayer") then
			
			local player = world:GetComponent(entityId, "TakeCardsFromPlayer", "Player"):GetInt()
			local ai = world:EntityHasComponent(player, "AI")
			local cards = self:GetEntities("DealtCard")

			for i = 1, #cards do
				
				local owner = world:GetComponent(cards[i], "DealtCard", "PlayerEntityId"):GetInt()
				
				if owner == player then
					
					world:RemoveComponentFrom("DealtCard", cards[i])
					
					if not ai then
						local ip = world:GetComponent(player, "NetConnection", "IpAddress"):GetText()
						local port = world:GetComponent(player, "NetConnection", "Port"):GetInt()

						Net.SendEntityKill(cards[i], ip, port)
					
					-- If the player which is no longer in the game is an ai, make sure to remove its cards.
					elseif world:EntityHasComponent(cards[i], "AICard") then
						world:RemoveComponentFrom("AICard", cards[i])
					end
				end
			end
			
			world:KillEntity( entityId )
			
			if ai then
				-- Kill the "ai-player"
				world:KillEntity(player)
			end
		end
	end
end
