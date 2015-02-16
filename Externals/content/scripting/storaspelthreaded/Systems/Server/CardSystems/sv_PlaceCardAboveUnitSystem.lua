PlaceCardAboveUnitSystem = System()
PlaceCardAboveUnitSystem.CardEntity = -1

PlaceCardAboveUnitSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PlaceCardAboveUnitSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PlayCard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardStep", FilterType.RequiresOneOf)
end

PlaceCardAboveUnitSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "PlayCard") then
			if self.CardEntity ~= -1 then
				world:KillEntity(self.CardEntity)
				self.CardEntity = -1
			end
		  
			local step = world:GetComponent( entity, "PlayCard", "Step"):GetInt()

			local cards = self:GetEntities("CardStep")
			
			local card = self:GetNextCard( cards, step )
			if card then
				local unit = world:GetComponent( card, "CardStep", "UnitEntityId"):GetInt()
				
				if not world:EntityHasComponent( unit, "UnitDead" ) then
				  
					local action = world:GetComponent( card, "CardAction", "Action"):GetText()

					self.CardEntity = world:CreateNewEntity()
					world:CreateComponentAndAddTo("Model", self.CardEntity)
					world:CreateComponentAndAddTo("Position", self.CardEntity)
					world:CreateComponentAndAddTo("Rotation", self.CardEntity)
					world:CreateComponentAndAddTo("Scale", self.CardEntity)
					world:CreateComponentAndAddTo("SyncNetwork", self.CardEntity)
					world:CreateComponentAndAddTo("Parent", self.CardEntity)
					world:CreateComponentAndAddTo("WorldToViewSpace", self.CardEntity)
					
					local model = world:GetComponent(self.CardEntity, "Model", 0)
					if action == "Forward" then
						model:SetModel("forward", "cards", 2)
					elseif action == "Backward" then
						model:SetModel("back", "cards", 2)
					elseif action == "TurnRight" then
						model:SetModel("turnright", "cards", 2)
					elseif action == "TurnLeft" then
						model:SetModel("turnleft", "cards", 2)
					elseif action == "TurnAround" then
						model:SetModel("turnaround", "cards", 2)
					elseif action == "Sprint" then
						model:SetModel("sprint", "cards", 2)
					elseif action == "SlingShot" then
						model:SetModel("slingshot", "cards", 2)
					elseif action == "IWin" then
						model:SetModel("iwin", "cards", 2)
					else
						model:SetModel("forward", "cards", 2)
					end
					world:GetComponent(self.CardEntity, "Position", 0):SetFloat3(0.0, 2.0, 0.0)
					world:GetComponent(self.CardEntity, "Rotation", 0):SetFloat3(1.5 * math.pi, math.pi, 0.0)
					world:GetComponent(self.CardEntity, "Scale", 0):SetFloat3(1.5, 1.5, 1.5)
					world:GetComponent(self.CardEntity, "Parent", 0):SetInt(unit)
				end
			end
		end
	end
end

PlaceCardAboveUnitSystem.GetNextCard = function(self, cards, step)
	
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
