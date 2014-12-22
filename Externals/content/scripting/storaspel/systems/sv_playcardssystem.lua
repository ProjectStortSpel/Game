PlayCardsSystem = System()

PlayCardsSystem.Initialize = function(self)
	self:SetName("PlayCardsSystem System")
	self:AddComponentTypeToFilter("PlayCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardStep", FilterType.RequiresOneOf)
	print("PlayCardsSystem initialized!")
end


PlayCardsSystem.Update = function(self, dt)
	
	

	local playCards = self:GetEntities("PlayCards")
	if #playCards > 0 then
		local step = world:GetComponent( playCards[1], "PlayCards", "Step"):GetInt()

		local cards = self:GetEntities("CardStep")
		local card = self:GetNextCard( cards, step )

		if card then
			local unit = world:GetComponent( card, "CardStep", "UnitEntityId"):GetInt()

			local cardAction = "Unit" .. world:GetComponent( card, "CardAction", "Action"):GetString()

			world:CreateComponentAndAddTo(cardAction, unit)


			world:RemoveComponentFrom("CardStep", card)
			world:CreateComponentAndAddTo("UsedCard", card)

		else
				
			--remove PlayCards
			for i = 1, #playCards do
				world:KillEntity( playCards[i] )
			end

			--create MoveRiver
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("MoveRiver", id)

		end
		
	end
end


PlayCardsSystem.GetNextCard = function(self, cards, step)
	
	local card = nil
	local prio = 9999
	for i = 1, #cards do
		
		local cardStep = world:GetComponent( cards[i], "CardStep", "Step"):GetInt()

		if cardStep == step then
			local tmp = world:GetComponent( cards[i], "CardPrio", "Prio"):GetInt()

			if tmp < prio then
				card = cards[i]
				prio = tmp
			end
		end
	end

	return card

end
