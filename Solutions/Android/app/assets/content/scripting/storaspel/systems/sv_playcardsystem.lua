PlayCardSystem = System()

PlayCardSystem.Initialize = function(self)
	self:SetName("PlayCardSystem System")
	self:AddComponentTypeToFilter("PlayCard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardStep", FilterType.RequiresOneOf)
	print("PlayCardSystem initialized!")
end


PlayCardSystem.OnEntityAdded = function(self, entity)

	if world:EntityHasComponent(entity, "PlayCard") then
		local step = world:GetComponent( entity, "PlayCard", "Step"):GetInt()

		local cards = self:GetEntities("CardStep")

		local card = self:GetNextCard( cards, step )

		if card then
			local unit = world:GetComponent( card, "CardStep", "UnitEntityId"):GetInt()

			local time = 0
			
			if not world:EntityHasComponent( unit, "UnitDead" ) then
				local cardAction = "Unit" .. world:GetComponent( card, "CardAction", "Action"):GetString()
				world:CreateComponentAndAddTo(cardAction, unit)

				time = 0.3
			end

			world:RemoveComponentFrom("CardStep", card)
			--world:CreateComponentAndAddTo("UsedCard", card)

			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("PlayCardTimer", id)
			world:GetComponent(id, "PlayCardTimer", "Time"):SetFloat(time)
			world:SetComponent(id, "PlayCardTimer", "Step", step)

		else
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("MoveRiver", id)

			id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("StepTimer", id)
			world:GetComponent(id, "StepTimer", "Time"):SetFloat(1)
			
		end

		world:KillEntity( entity )
	end
end


PlayCardSystem.GetNextCard = function(self, cards, step)
	
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
