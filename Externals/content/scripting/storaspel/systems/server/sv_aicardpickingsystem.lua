AiCardPickingSystem = System()
AiCardPickingSystem.NumberOfCardsToPick = 5
AiCardPickingSystem.CardsPerHand = 8

AiCardPickingSystem.Initialize = function(self)
	self:SetName("AI card picking System")
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AICard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp", FilterType.RequiresOneOf)
	
end

AiCardPickingSystem.Update = function(self, dt)
	
	local AIs = self:GetEntities("AI")
	local Cards = self:GetEntities("AICard")
	local CPtiles = self:GetEntities("Checkpoint")
	
	for i = 1, #AIs do
		
		local unitID = self:GetComponent(AIs[i], "UnitEntityId", 0):GetInt()
		
		local cpTargetNr = self:GetComponent(unitID, "TargetCheckpoint", 0):GetInt()
		
		-- vart AIn vill
		local targetPositionX, targetPositionY = self:GetTargetPosition(CPtiles, cpTargetNr)
		-- vart AIn är
		local aiPositonX, aiPositonY = self:GetComponent(unitID, "MapPosition", 0):GetInt2()
		-- AIs direction
		local aiDirX, aiDirY = self:GetComponent(unitID, "Direction", 0):GetInt2()
		--Fetch the cards which is relevant to the current AI
		local CardSetAI = self:GetAIsCardSet(AIs[i], Cards)
		--This will catch the best 
		local PickedCards = self:AIPickCards(CardSetAI)

		if #PickedCards >= self.NumberOfCardsToPick then	
			self:SendCards(PickedCards, AIs[i])
		end
	end
end

AiCardPickingSystem.GetTargetPosition = function(self, checkpointsTiles, cpTargetNr)
	local targetPositionX, targetPositionY
	
	for k = 1, #checkpointsTiles do
		local target = self:GetComponent(checkpointsTiles[k], "Checkpoint", 0):GetInt()
		
		if target == cpTargetNr then
			targetPositionX, targetPositionY = self:GetComponent(checkpointsTiles[k], "MapPosition", 0):GetInt2()

		end
	end
	
	return targetPositionX, targetPositionY
end

AiCardPickingSystem.GetAIsCardSet = function(self, AI, Cards)

		local aisCard = {}
		local aiNr = self:GetComponent(AI, "PlayerNumber", 0):GetInt()
		for j = 1, #Cards do
			local card = self:GetComponent(Cards[j], "DealtCard", 0)
			local id = card:GetInt()
			local plyNr = self:GetComponent(id, "PlayerNumber", 0):GetInt()

			if plyNr == aiNr then
				
				aisCard[#aisCard+1] = Cards[j]
			end
		end
		
		return aisCard
end

AiCardPickingSystem.AIPickCards = function( self, CardSetAI )
	
	local pickedcards = {}
	if #CardSetAI >= 5 then
		for i = 1, 5 do
			local cardNr = math.random(1, #CardSetAI)
			
			local pickedcard = CardSetAI[cardNr]
			
			pickedcards[#pickedcards + 1] = pickedcard
			
			table.remove(CardSetAI, cardNr)
		end
	end
	return pickedcards
end

AiCardPickingSystem.SendCards = function(self, pickedcards, player)
	
	local unit = world:GetComponent(player, "UnitEntityId", "Id"):GetInt()
	
	world:CreateComponentAndAddTo("HasSelectedCards", player)
	world:CreateComponentAndAddTo("UnitSelectedCards", unit)
	for i = 1, self.NumberOfCardsToPick do
		local action = world:GetComponent(pickedcards[i], "CardAction", 0):GetString()
		local prio = world:GetComponent(pickedcards[i], "CardPrio", 0):GetInt()
		print("AI Action: " .. action .. " - Prio: " .. prio)
	
		world:RemoveComponentFrom("DealtCard", pickedcards[i])
		world:CreateComponentAndAddTo("CardStep", pickedcards[i])
		world:SetComponent(pickedcards[i], "CardStep", "Step", i)
		world:SetComponent(pickedcards[i], "CardStep", "UnitEntityId", unit)
		
	end
end

AiCardPickingSystem.OnEntityAdded = function(self, entity)

	if world:EntityHasComponent(entity, "DealtCard") then
		local playerid = self:GetComponent(entity, "DealtCard", 0)
		local id = playerid:GetInt()
		local plynum = self:GetComponent(id, "PlayerNumber", 0):GetInt()
		local card = self:GetComponent(entity, "CardAction", 0):GetString()
		--print ( plynum .. " gets a " .. card .. " Card" )
	elseif world:EntityHasComponent(entity, "TileComp") then
	
	end

end
