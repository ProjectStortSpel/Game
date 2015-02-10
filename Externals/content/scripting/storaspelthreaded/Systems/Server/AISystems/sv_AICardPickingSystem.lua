AICardPickingSystem = System()
AICardPickingSystem.NumberOfCardsToPick = 5
AICardPickingSystem.CardsPerHand = 8
AICardPickingSystem.PrintSimulation = 0
AICardPickingSystem.AICheat = 0

AICardPickingSystem.Initialize = function(self)
	self:SetName("AI card picking System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AICard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	
	--Console.AddCommand("AICheat", self.CheatMode)
end

--AddAISystem.CheatMode = function(_command, ...)
--	
--	local args = { ... }
--	
--	if #args == 1 then
--		if type(args[1]) == "number" then
--			if -1 < args[1] and args[1] < 2 then
--				self.AICheat = args[1]
--			end
--		end
--	else
--		if self.AICheat == 0 then
--			self.AICheat = 1
--		else
--			self.AICheat = 0
--		end
--	end
--	
--	if self.AICheat == 0 then
--		print("AICheat off")
--	else
--		print("AICheat on")
--	end
--end

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
			local PickedCards = self:AIPickCards(CardSetAI, aiDirX, aiDirY, aiPositonX, aiPositonY, targetPositionX, targetPositionY, unitID)
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
		aisCard.__mode = "k"
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

AICardPickingSystem.TryMove = function(self, CardSetAI, card)
	--print(#card)
	local cardpicked
	for j = 1, #CardSetAI do
		if CardSetAI[j] == card[1] and j <= #CardSetAI then
			table.remove(card, 1)
			cardpicked = CardSetAI[j]
			--print("PLAYING CARD : ", world:GetComponent(cardpicked, "CardAction", 0):GetString()))
			table.remove(CardSetAI, j)
			return cardpicked
		end
	end
end

AICardPickingSystem.AIPickCards = function( self, CardSetAI, dirX, dirY, posX, posY, targetX, targetY, unitID )
	
	local pickedcards = {}
	pickedcards.__mode = "k"
	if #CardSetAI >= self.CardsPerHand then
		
		if self.AICheat == 1 then
			
			for j = 1, self.CardsPerHand do
				
				local cardactioncomp = world:GetComponent(CardSetAI[j], "CardAction", 0)
				
				if j == 1 then
					cardactioncomp:SetString("Forward")
				elseif j == 2 then
					cardactioncomp:SetString("Forward")
				elseif j == 3 then
					cardactioncomp:SetString("Backward")
				elseif j == 4 then
					cardactioncomp:SetString("Backward")
				elseif j == 5 then
					cardactioncomp:SetString("TurnRight")
				elseif j == 6 then
					cardactioncomp:SetString("TurnLeft")
				elseif j == 7 then
					cardactioncomp:SetString("TurnAround")
				elseif j == 8 then
					cardactioncomp:SetString("Forward")
				end
			end
		end

		local forwards = self:GetAllCardsOf(CardSetAI, "Forward")
		local backwards = self:GetAllCardsOf(CardSetAI, "Backward")
		local turnLefts = self:GetAllCardsOf(CardSetAI, "TurnLeft")
		local turnRights = self:GetAllCardsOf(CardSetAI, "TurnRight")
		local turnArounds = self:GetAllCardsOf(CardSetAI, "TurnAround")
		local sprints = self:GetAllCardsOf(CardSetAI, "Sprint")
		local shots = self:GetAllCardsOf(CardSetAI, "SlingShot")
		
		for i = 1, self.NumberOfCardsToPick do
			--SimulatePlayOfCards
			
			local cpTargetNr = world:GetComponent(unitID, "TargetCheckpoint", 0):GetInt()
			local CPtiles = self:GetEntities("Checkpoint")
			targetPositionX, targetPositionY = self:GetTargetPosition(CPtiles, cpTargetNr)
			
			
			local cardstosim = {}
			cardstosim.__mode = "k"
	
			table.insert(cardstosim, "Forward")
			
			local test_x, test_y, dir_test_x, dir_test_y = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim)
			local from_forward  = PathfinderHandler.GeneratePath(test_x, test_y, targetX, targetY)
			
			local test_x, test_y, dir_test_x, dir_test_y = self:SimpleSimulatePlayOfCards( unitID, posX+dirX, posY+dirY, dirX, dirY, cardstosim)
			local from_sprint = PathfinderHandler.GeneratePath(test_x, test_y, targetX, targetY)
			
			local cardstosim2 = {}
			cardstosim2.__mode = "k"
			table.insert(cardstosim2, "Backward")
			
			local test_x, test_y, dir_test_x, dir_test_y = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim2)
			local from_backward  = PathfinderHandler.GeneratePath(test_x, test_y, targetX, targetY)
			
			local cardstosim3 = {}
			cardstosim3.__mode = "k"
			table.insert(cardstosim3, "TurnLeft")
			table.insert(cardstosim3, "Forward")
            
			local test_x, test_y, dir_test_x, dir_test_y = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim3)
			local from_left = PathfinderHandler.GeneratePath(test_x, test_y, targetX, targetY)
			
			local cardstosim4 = {}
			cardstosim4.__mode = "k"
			table.insert(cardstosim4, "TurnRight")
			table.insert(cardstosim4, "Forward")
			
			local test_x, test_y, dir_test_x, dir_test_y = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim4)
			local from_right = PathfinderHandler.GeneratePath(test_x, test_y, targetX, targetY)
			local from_right = PathfinderHandler.GeneratePath(test_x, test_y, targetX, targetY)
			--forward_play = 
			
			local from_me = PathfinderHandler.GeneratePath(posX, posY, targetX, targetY)
			--local form_x, from_y = posX+dirX, posY+dirY;
			--local from_forward = PathfinderHandler.GeneratePath(form_x, from_y, targetX, targetY)
			--local form_x, from_y = posX+dirX+dirX, posY+dirY+dirY;
			--local from_sprint = PathfinderHandler.GeneratePath(form_x, from_y, targetX, targetY)
			--form_x, from_y = posX-dirX, posY-dirY;
			--local from_backward = PathfinderHandler.GeneratePath(form_x, from_y, targetX, targetY)
			--form_x, from_y = posX+dirY, posY-dirX;
			--local from_left = PathfinderHandler.GeneratePath(form_x, from_y, targetX, targetY)
			--form_x, from_y = posX-dirY, posY+dirX;
			--local from_right = PathfinderHandler.GeneratePath(form_x, from_y, targetX, targetY)
			
			--print( from_forward_sim, from_forward)
			print("Position : " .. posX .. ", " .. posY ..  "(x , y)")
			print("Direction: " .. dirX .. ", " .. dirY .. "(x , y)")
			print("distance from current position		: ", from_me)
			print("distance from position (forward)	: ", from_forward)
			print("distance from position (backward)	: ", from_backward)
			print("distance from position (left)		: ", from_left)
			print("distance from position (right)		: ", from_right)
			print("forwards    " .. #forwards)
			print("backwards   " .. #backwards)
			print("turnLefts   " .. #turnLefts)
			print("turnRights  " .. #turnRights)
			print("turnArounds " .. #turnArounds)
			
			
			if from_me > from_sprint and #sprints > 0 
			then 
				print("Sprint")
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, sprints)
				 posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX+dirX, posY+dirY, dirX, dirY, cardstosim)
				
			elseif from_me > from_forward and #forwards > 0 
			then 
				print("FORWARD")
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, forwards)
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim)
				
			elseif from_me > from_backward and #backwards > 0 
			then
				print("BACKWORD")
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, backwards)
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim2)
		
			elseif from_me > from_left and #turnLefts > 0
			then
				print("LEFT")
				if  #backwards > #forwards and #turnRights > 0 
				then
					print("FUCK IT LETS GO RIGHT")
					pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnRights)
					table.remove(cardstosim4, 2)
					posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim4)
				else
					pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnLefts)
					table.remove(cardstosim3, 2)
					posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim3)
				end
			elseif from_me > from_right and #turnRights > 0
			then
				print("RIGHT")
				if  #backwards > #forwards and #turnLefts > 0 
				then
					print("FUCK IT LETS GO LEFT")
					pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnLefts)
					table.remove(cardstosim3, 2)
					posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim3)
				else
					pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnRights)
					table.remove(cardstosim4, 2)
					posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim4)
				end
				
			elseif from_me > from_forward and #turnArounds > 0 and #forwards == 0 and #backwards > 0
			then
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnArounds)
				local cardstosim5 = {}
				cardstosim5.__mode = "k"
				table.insert(cardstosim5, "TurnAround")
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim5)
			elseif from_me > from_backward and #turnArounds > 0 and #forwards > 0 and #backwards == 0
			then
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnArounds)
				local cardstosim5 = {}
				cardstosim5.__mode = "k"
				table.insert(cardstosim5, "TurnAround")
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim5)
			elseif #shots > 0 then
				print("FIRE!")
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, shots)
				local cardstosim5 = {}
				cardstosim5.__mode = "k"
				table.insert(cardstosim5, "TurnAround")
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim5)
				dirX = -dirX
				dirY = -dirY
			elseif #turnLefts > 0 then
            
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnLefts)
				table.remove(cardstosim3, 2)
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim3)
            
			elseif #turnRights > 0 then
            
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnRights)
				table.remove(cardstosim3, 2)
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim3)
            
			elseif #turnArounds > 0 then
            
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, turnArounds)
				local cardstosim5 = {}
				cardstosim5.__mode = "k"
				table.insert(cardstosim5, "TurnAround")
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim5)
            
			elseif #forwards > 0 then
            
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, forwards)
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim)
            
			elseif #backwards > 0 then
            
				pickedcards[#pickedcards+1] = self:TryMove(CardSetAI, forwards)
				posX, posY, dirX, dirY = self:SimpleSimulatePlayOfCards( unitID, posX, posY, dirX, dirY, cardstosim2)
			else
				print("RANDOM!")
				local cardNr = math.random(1, #CardSetAI)

				local pickedCard = CardSetAI[cardNr]

				pickedcards[#pickedcards+1] = pickedCard
				
				local super = {}
				
				super.__mode = "k"
				
				table.insert(super, pickedCard) 
				
				posX, posY, dirX, dirY = self:SimulatePlayOfCards( unitID, super )

				table.remove(CardSetAI, cardNr)
			end

			
		end
	end
	return pickedcards
end

AICardPickingSystem.GetAllCardsOf = function( self, CardSetAI, cardName )
	
	local cards = {}
	cards.__mode = "k"
	for i = 1, #CardSetAI do

		local nameCard = world:GetComponent(CardSetAI[i], "CardAction", 0):GetString()

		if cardName == nameCard then
			cards[#cards + 1] = CardSetAI[i]
		end
	end
	return cards
end

AICardPickingSystem.SendCards = function(self, _player, _pickedcards)
	
	print("AI Send Cards")
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
			
		elseif cardName == "SlingShot" then
		
			print(posX, posY, dirX, dirY)
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 0)
			print(posX, posY, dirX, dirY)
			
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

AICardPickingSystem.SimpleSimulatePlayOfCards = function(self, _unit, posX, posY, dirX, dirY, _pickedcards)
	
	--local mapSize = self:GetEntities("MapSize")	
	--local mapX, mapY = world:GetComponent(mapSize[1], "MapSize", 0):GetInt2()
	--local posX, posY = world:GetComponent(_unit, "MapPosition", 0):GetInt2()
	--local dirX, dirY = world:GetComponent(_unit, "Direction", 0):GetInt2()
	
	local fellDown = false
	
	for i = 1, #_pickedcards do
		
		local cardName = _pickedcards[i]
		
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
	
	local mapSize = self:GetEntities("MapSpecs")
	local mapSizeComp = world:GetComponent(mapSize[1], "MapSpecs", "SizeX")
	local mapX, mapY = mapSizeComp:GetInt2()
	local tiles = self:GetEntities("TileComp")
	
	local returnValue = world:EntityHasComponent(tiles[mapX * _posY + _posX + 1], _component)
	return returnValue
end

AICardPickingSystem.GetRiverVariables = function(self, _posX, _posY)
	
	local mapSize = self:GetEntities("MapSpecs")
	local mapX = world:GetComponent(mapSize[1], "MapSpecs", "SizeX"):GetInt()
	local tiles = self:GetEntities("TileComp")
	local dirX, dirY, speed = world:GetComponent(tiles[mapX * _posY + _posX + 1], "River", 0):GetInt3()
	
	return dirX, dirY, speed
end

AICardPickingSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for i = 1, #entities do
		if world:EntityHasComponent(entities[i], "DealtCard") then
			local playerid = world:GetComponent(entities[i], "DealtCard", 0)
			local id = playerid:GetInt()
			local plynum = world:GetComponent(id, "PlayerNumber", 0):GetInt()
			local card = world:GetComponent(entities[i], "CardAction", 0):GetString()
		end
	end
end