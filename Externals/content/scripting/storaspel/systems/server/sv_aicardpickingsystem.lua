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
		local PickedCards = self:AIPickCards(CardSetAI, aiDirX, aiDirY, aiPositonX, aiPositonY, targetPositionX, targetPositionY)

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
AiCardPickingSystem.TryMove = function(self, CardSetAI, card)
	print(#card)
	local cardpicked
	for j = 1, #CardSetAI do
		if CardSetAI[j] == card[1] and j <= #CardSetAI then
			table.remove(card, 1)
			cardpicked = CardSetAI[j]
			table.remove(CardSetAI, j)
			return cardpicked
		end
	end
end
AiCardPickingSystem.AIPickCards = function( self, CardSetAI, dirX, dirY, posX, posY, targetX, targetY )
	
	local pickedcards = {}
	if #CardSetAI >= 5 then
		local forwards = self:GetAllCardsOf(CardSetAI, "Forward")
		local backwards = self:GetAllCardsOf(CardSetAI, "Backward")
		local turnLefts = self:GetAllCardsOf(CardSetAI, "TurnLeft")
		local turnRights = self:GetAllCardsOf(CardSetAI, "TurnRight")
		local turnArounds = self:GetAllCardsOf(CardSetAI, "TurnAround")

		--print("Position : " .. posX .. ", " .. posY ..  "(x , y)")
		--print("Target   : " .. targetX .. ", " .. targetY .. "(x , y)")
		--print("forwards    " .. #forwards)
		--print("backwards   " .. #backwards)
		--print("turnLefts   " .. #turnLefts)
		--print("turnRights  " .. #turnRights)
		--print("turnArounds " .. #turnArounds)

		for i = 1, 5 do
			
			if posY < targetY and dirY == 1 and #forwards > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, forwards)
				posY = posY + 1
			
			elseif posY < targetY and dirY == -1 and #backwards > 0 then
			
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, backwards)
				posY = posY + 1
			
			elseif posY > targetY and dirY == -1 and #forwards > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, forwards)
				posY = posY - 1
			
			elseif posY > targetY and dirY == 1 and #backwards > 0 then
			
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, backwards)
				posY = posY - 1
			
			elseif posX < targetX and dirX == 1 and #forwards > 0 then
			
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, forwards)
				posX = posX + 1
			
			elseif posX < targetX and dirX == -1 and #backwards > 0 then
				
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, backwards)
				posX = posX + 1
			
			elseif posX > targetX and dirX == -1 and #forwards > 0 then
			
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, forwards)
				posX = posX - 1
			
			elseif posX > targetX and dirX == 1 and #backwards > 0 then
				
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, backwards)
				posX = posX - 1
			
			elseif posX > targetX and dirX == 1 and #turnArounds > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnArounds)
				dirX = -dirX
				dirY = -dirY

			elseif posX < targetX and dirX == -1 and #turnArounds > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnArounds)
				dirX = -dirX
				dirY = -dirY

			elseif posY < targetY and dirY == -1 and #turnArounds > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnArounds)
				dirX = -dirX
				dirY = -dirY

			elseif posY > targetY and dirY == 1 and #turnArounds > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnArounds)
				dirX = -dirX
				dirY = -dirY

			elseif #turnLefts > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnLefts)
				local temp = dirY
				dirY = -dirX
				dirX = temp

			elseif #turnRights > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnRights)
				local temp = dirY
				dirY = dirX
				dirX = -temp

			elseif #turnArounds > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnArounds)
				dirX = -dirX
				dirY = -dirY

			elseif #forwards > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, forwards)
				posX = posX + dirX
				posY = posY + dirY

			elseif #backwards > 0 then

				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, backwards)
				posX = posX - dirX
				posY = posY - dirY

			else
				local cardNr = math.random(1, #CardSetAI)

				local pickedCard = CardSetAI[cardNr]

				pickedcards[#pickedcards+1] = pickedCard

				table.remove(CardSetAI, cardNr)
			end

			
			
		end
	end
	return pickedcards
end

AiCardPickingSystem.GetAllCardsOf = function( self, CardSetAI, cardName )
	
	local cards = {}
	for i = 1, #CardSetAI do

		local nameCard = self:GetComponent(CardSetAI[i], "CardAction", 0):GetString()

		if cardName == nameCard then
			cards[#cards + 1] = CardSetAI[i]
		end
	end
	return cards
end

AiCardPickingSystem.SendCards = function(self, pickedcards, player)
	
	print("DONE")
	local unit = world:GetComponent(player, "UnitEntityId", "Id"):GetInt()
	
	world:CreateComponentAndAddTo("HasSelectedCards", player)
	world:CreateComponentAndAddTo("UnitSelectedCards", unit)

	for i = 1, self.NumberOfCardsToPick do
		local action = world:GetComponent(pickedcards[i], "CardAction", 0):GetString()
		local prio = world:GetComponent(pickedcards[i], "CardPrio", 0):GetInt()
		--print("AI Action: " .. action .. " - Prio: " .. prio)
	
		world:RemoveComponentFrom("DealtCard", pickedcards[i])
		world:RemoveComponentFrom("AICard", pickedcards[i])
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