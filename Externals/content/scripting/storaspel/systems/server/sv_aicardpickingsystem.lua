AICardPickingSystem = System()
AICardPickingSystem.NumberOfCardsToPick = 5
AICardPickingSystem.CardsPerHand = 8
AICardPickingSystem.PrintSimulation = 0

AICardPickingSystem.Initialize = function(self)
	self:SetName("AI card picking System")
	
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AICard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSize", FilterType.RequiresOneOf)
	
end

AICardPickingSystem.Update = function(self, dt)
	
	local AIs = self:GetEntities("AI")
	local Cards = self:GetEntities("AICard")
	
	if #Cards >= #AIs * self.CardsPerHand then
	
		local CPtiles = self:GetEntities("Checkpoint")
		
		for i = 1, #AIs do
			
			local unitID = world:GetComponent(AIs[i], "UnitEntityId", 0):GetInt()
			
			local cpTargetNr = world:GetComponent(unitID, "TargetCheckpoint", 0):GetInt()
					
			-- vart AIn vill
			local targetPositionX, targetPositionY = self:GetTargetPosition(CPtiles, cpTargetNr)
			--local targetPositionX, targetPositionY = 5, 10
			-- vart AIn är
			local aiPositonX, aiPositonY = world:GetComponent(unitID, "MapPosition", 0):GetInt2()
			-- AIs direction
			local aiDirX, aiDirY = world:GetComponent(unitID, "Direction", 0):GetInt2()
			--Fetch the cards which is relevant to the current AI
			local CardSetAI = self:GetAIsCardSet(AIs[i], Cards)
			--This will catch the best 
			local PickedCards = self:AIPickCards(CardSetAI, aiDirX, aiDirY, aiPositonX, aiPositonY, targetPositionX, targetPositionY)
			aiPositonX, aiPositonY, aiDirX, aiDirY = self:SimulatePlayOfCards(unitID, PickedCards)

			if #PickedCards >= self.NumberOfCardsToPick then	
				
				self:SendCards(AIs[i], PickedCards)
			end
		end
	end
end

AICardPickingSystem.GetTargetPosition = function(self, checkpointsTiles, cpTargetNr)
	local targetPositionX, targetPositionY
	local asd = false
	for k = 1, #checkpointsTiles do
		local target = world:GetComponent(checkpointsTiles[k], "Checkpoint", 0):GetInt()
		
		if target == cpTargetNr then
			targetPositionX, targetPositionY = world:GetComponent(checkpointsTiles[k], "MapPosition", 0):GetInt2()
			asd = true
		end
	end
	if asd == false then
		targetPositionX = 0
		targetPositionY = 0
	end
	
	return targetPositionX, targetPositionY
end

AICardPickingSystem.GetAIsCardSet = function(self, AI, Cards)

		local aisCard = {}
		local aiNr = world:GetComponent(AI, "PlayerNumber", 0):GetInt()
		for j = 1, #Cards do
			local card = world:GetComponent(Cards[j], "DealtCard", 0)
			local id = card:GetInt()
			local plyNr = world:GetComponent(id, "PlayerNumber", 0):GetInt()

			if plyNr == aiNr then
				
				aisCard[#aisCard+1] = Cards[j]
			end
		end
		
		return aisCard
end
AiCardPickingSystem.TryMove = function(self, CardSetAI, card)
	--print(#card)

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

		for i = 1, 5 do
			--print("Position : " .. posX .. ", " .. posY ..  "(x , y)")
			--print("Target   : " .. targetX .. ", " .. targetY .. "(x , y)")
			--print("Direction: " .. dirX .. ", " .. dirY .. "(x , y)")
			--print("forwards    " .. #forwards)
			--print("backwards   " .. #backwards)
			--print("turnLefts   " .. #turnLefts)
			--print("turnRights  " .. #turnRights)
			--print("turnArounds " .. #turnArounds)

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

AICardPickingSystem.GetAllCardsOf = function( self, CardSetAI, cardName )
	
	local cards = {}
	for i = 1, #CardSetAI do

		local nameCard = world:GetComponent(CardSetAI[i], "CardAction", 0):GetString()

		if cardName == nameCard then
			cards[#cards + 1] = CardSetAI[i]
		end
	end
	return cards
end

AICardPickingSystem.SendCards = function(self, _player, _pickedcards)
	
	print("AI DONE")
	local unit = world:GetComponent(_player, "UnitEntityId", "Id"):GetInt()
	
	world:CreateComponentAndAddTo("HasSelectedCards", _player)
	world:CreateComponentAndAddTo("UnitSelectedCards", unit)

	for i = 1, self.NumberOfCardsToPick do
		local action = world:GetComponent(_pickedcards[i], "CardAction", 0):GetString()
		local prio = world:GetComponent(_pickedcards[i], "CardPrio", 0):GetInt()
		--print("AI Action: " .. action .. " - Prio: " .. prio)
	
		world:RemoveComponentFrom("DealtCard", _pickedcards[i])
		world:RemoveComponentFrom("AICard", _pickedcards[i])
		world:CreateComponentAndAddTo("CardStep", _pickedcards[i])
		world:SetComponent(_pickedcards[i], "CardStep", "Step", i)
		world:SetComponent(_pickedcards[i], "CardStep", "UnitEntityId", unit)
				
	end

	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("NotifyStartNewRound", id)
	world:GetComponent(id, "NotifyStartNewRound", "IsAI"):SetBool(true) 
end

AICardPickingSystem.SimulatePlayOfCards = function(self, _unit, _pickedcards)
	
	--local mapSize = self:GetEntities("MapSize")	
	--local mapX, mapY = world:GetComponent(mapSize[1], "MapSize", 0):GetInt2()
	local posX, posY = world:GetComponent(_unit, "MapPosition", 0):GetInt2()
	local dirX, dirY = world:GetComponent(_unit, "Direction", 0):GetInt2()
	
	local fellDown = false
	
	for i = 1, #_pickedcards do
		
		local cardName = world:GetComponent(_pickedcards[i], "CardAction", 0):GetString()
		
		if cardName == "Forward" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, 1, false)
			
		elseif cardName == "Backward" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, false, 1, false)
			
		elseif cardName == "TurnLeft" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 1)
			
		elseif cardName == "TurnRight" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 3)
			
		elseif cardName == "TurnAround" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 2)
			
		else
		
			print("ERROR: CARD NOT ADDED IN SIMULATE CARDS", cardName)
		end
		
		if fellDown then
			
			posX, posY = world:GetComponent(_unit, "Spawnpoint", 0):GetInt2()
			break
		end
	end
	
	if self.PrintSimulation == 1 then
		print("I will end up at:", posX, posY, "with dir:", dirX, dirY)
	end
	
	return posX, posY, dirX, dirY
end

AICardPickingSystem.SimulateMoveForward = function(self, _posX, _posY, _dirX, _dirY, _forwards, _iterations, _riverMove)
	
	local forward = -1
	if _forwards then
		forward = 1
	end
	
	local fellDown = false
	local posX, posY
			
	for i = 1, _iterations do
		
		posX = _posX + _dirX * forward
		posY = _posY + _dirY * forward
	
		if self:TileHasComponent("Void", posX, posY) then
			
			fellDown = true
			if self.PrintSimulation == 1 then
				print("I will fall down in", posX, posY)
			end
			
		elseif self:TileHasComponent("River", posX, posY) and not _riverMove then
			
			local waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(posX, posY)
			
			if self.PrintSimulation == 1 then
				print("I will move in river with X, Y, speed:", waterDirX, waterDirY, waterSpeed)
			end
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, waterDirX, waterDirY, true, waterSpeed, true)
			
		elseif self:TileHasComponent("NotWalkable", posX, posY) then
			
			posX = _posX
			posY = _posY
		end
	end
	
	if not _riverMove and self.PrintSimulation == 1 then
		print("Pos", posX, posY, "Forw:", _forwards)
	end
	
	return fellDown, posX, posY
end

AICardPickingSystem.SimulateTurnLeft = function(self, _posX, _posY, _dirX, _dirY, _iterations)
	
	local dirX = _dirX
	local dirY = _dirY
	local temp
	local posX = _posX
	local posY = _posY
	local fellDown = false
	
	for i = 1, _iterations do
		
		temp = dirY
		dirY = -dirX
		dirX = temp
	end
	
	if self:TileHasComponent("River", _posX, _posY) then
		
		local waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(_posX, _posY)
		
		if self.PrintSimulation == 1 then
			print("I will move in river with X, Y, speed:", waterDirX, waterDirY, waterSpeed)
		end
		
		fellDown, posX, posY = self:SimulateMoveForward(_posX, _posY, waterDirX, waterDirY, true, waterSpeed, true)
		
		if self.PrintSimulation == 1 then
			print("Pos", posX, posY, "Dir", dirX, dirY, "with", _iterations)
		end
	elseif self.PrintSimulation == 1 then
		print("Dir", dirX, dirY, "with", _iterations)
	end
	
	return fellDown, posX, posY, dirX, dirY
end

AICardPickingSystem.TileHasComponent = function(self, _component, _posX, _posY)
	
	local mapSize = self:GetEntities("MapSize")
	local mapSizeComp = world:GetComponent(mapSize[1], "MapSize", 0)
	local mapX, mapY = mapSizeComp:GetInt2()
	local tiles = self:GetEntities("TileComp")
	
	local returnValue = world:EntityHasComponent(tiles[mapX * _posY + _posX + 1], _component)
	return returnValue
end
AiCardPickingSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for i = 0, #entities do
		if world:EntityHasComponent(entities[i], "DealtCard") then
			local playerid = world:GetComponent(entities[i], "DealtCard", 0)
			local id = playerid:GetInt()
			local plynum = world:GetComponent(id, "PlayerNumber", 0):GetInt()
			local card = world:GetComponent(entities[i], "CardAction", 0):GetString()
			--print ( plynum .. " gets a " .. card .. " Card" )
		elseif world:EntityHasComponent(entities[i], "TileComp") then
		
		end
	end
end
AICardPickingSystem.GetRiverVariables = function(self, _posX, _posY)
	
	local mapSize = self:GetEntities("MapSize")
	local mapX = world:GetComponent(mapSize[1], "MapSize", 0):GetInt()
	local tiles = self:GetEntities("TileComp")
	local dirX, dirY, speed = world:GetComponent(tiles[mapX * _posY + _posX + 1], "River", 0):GetInt3()
	
	return dirX, dirY, speed
end