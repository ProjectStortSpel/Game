PlayCardSystem = System()

PlayCardSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PlayCardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PlayCard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardStep", FilterType.RequiresOneOf)
end


PlayCardSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "PlayCard") then
			local step = world:GetComponent( entity, "PlayCard", "Step"):GetInt()

			local cards = self:GetEntities("CardStep")

			local card = self:GetNextCard( cards, step )

			if card then
				local unit = world:GetComponent( card, "CardStep", "UnitEntityId"):GetInt()

				local time = 0
				
				if not world:EntityHasComponent( unit, "UnitDead" ) then
					
					local plyNum = world:GetComponent( unit, "PlayerNumber", "Number" ):GetInt()
					local action = world:GetComponent( card, "CardAction", "Action"):GetString()

					local file = File.Append("gamelog.txt")
					File.WriteLine(file, "PlayCard " .. plyNum .. " " .. action)
					File.Close(file)

					local cardAction = "Unit" .. action
					world:CreateComponentAndAddTo(cardAction, unit)
					time = 1.5
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
