AICardPickingSystem = System()
AICardPickingSystem.NumberOfCardsToPick = 5
AICardPickingSystem.CardsPerHand = 8
AICardPickingSystem.PrintSimulation = 0
AICardPickingSystem.AICheat = 1
AICardPickingSystem.CardsToSimulate = 3

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
	--local PickedCards = {}
	--PickedCards.__mode = "k"
	
	if #Cards >= #AIs * self.CardsPerHand then
	
		local CPtiles = self:GetEntities("Checkpoint")
		
		for i = 1, #AIs do
			
			PotentialFieldHandler.UseMyPF(i)
			
			--local charArray = CombinationMath.Combinations(self.CardsPerHand, self.NumberOfCardsToPick)
			
			local charArray = CombinationMath.Permutations(self.CardsPerHand, self.CardsToSimulate)
			
			--local charArray = CombinationMath.Permutations(3, 2)
			
			--print(string.len(charArray))
			
			for n = 1, string.len(charArray), self.NumberOfCardsToPick do
				
				for card = 0, self.NumberOfCardsToPick - 1 do
				
					local charVar = string.byte(charArray, n + card)
					--io.write(charVar, " ")
				end
				--io.write("\n")
			end
			
			--for n = 1, string.len(charArray), 2 do
			--	
			--	for card = 0, 1 do
			--	
			--		local charVar = string.byte(charArray, n + card)
			--		print(charVar)
			--	end
			--end
			
			local unitID = world:GetComponent(AIs[i], "UnitEntityId", 0):GetInt()
			
			local cpTargetNr = world:GetComponent(unitID, "TargetCheckpoint", 0):GetInt()
					
			-- vart AIn vill
			local targetPositionX, targetPositionY = self:GetTargetPosition(CPtiles, cpTargetNr)
			
			-- vart AIn är
			local aiPositionX, aiPositionY = world:GetComponent(unitID, "MapPosition", 0):GetInt2()
			-- AIs direction
			local aiDirX, aiDirY = world:GetComponent(unitID, "Direction", 0):GetInt2()
			local fellDown
			--Fetch the cards which is relevant to the current AI
			local CardSetAI = self:GetAIsCardSet(AIs[i], Cards)
			
			if #CardSetAI == self.CardsPerHand then
				--This will catch the best 
				local PickedCards = self:AIPickCards(CardSetAI, aiDirX, aiDirY, aiPositionX, aiPositionY, targetPositionX, targetPositionY, unitID)
				fellDown, aiPositionX, aiPositionY, aiDirX, aiDirY = self:SimulatePlayOfCards(unitID, PickedCards)
   
				if #PickedCards == self.NumberOfCardsToPick then	
					
					self:SendCards(AIs[i], PickedCards)
				else
					print("ERROR: AI HAVE NOT SELECTED ENOUGH CARDS", #PickedCards)
				end
			else
				print("AI has not received enough cards yet. It has", #CardSetAI, "cards")
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

	local cardpicked
	for j = 1, #CardSetAI do
		if CardSetAI[j] == card[1] and j <= #CardSetAI then
			table.remove(card, 1)
			cardpicked = CardSetAI[j]
			--print("PLAYING CARD : ", world:GetComponent(cardpicked, "CardAction", 0):GetText()))
			table.remove(CardSetAI, j)
			return cardpicked
		end
	end
end

AICardPickingSystem.AIPickCards = function( self, CardSetAI, _dirX, _dirY, _posX, _posY, _targetX, _targetY, _unitID )
	
	local pickedCards = {}
	pickedCards.__mode = "k"
	
	--print(#CardSetAI, _dirX, _dirY, _posX, _posY, _targetX, _targetY, _unitID)
	
	if #CardSetAI >= self.CardsPerHand then
		
		if self.AICheat == 1 then
			
			for j = 1, self.CardsPerHand do
				
				local cardactioncomp = world:GetComponent(CardSetAI[j], "CardAction", 0)
				
				if j == 1 then
					cardactioncomp:SetText("Forward")
				elseif j == 2 then
					cardactioncomp:SetText("Forward")
				elseif j == 3 then
					--cardactioncomp:SetText("Backward")
					cardactioncomp:SetText("Forward")
				elseif j == 4 then
					--cardactioncomp:SetText("Backward")
					cardactioncomp:SetText("Forward")
				elseif j == 5 then
					cardactioncomp:SetText("TurnRight")
				elseif j == 6 then
					cardactioncomp:SetText("TurnLeft")
				elseif j == 7 then
					cardactioncomp:SetText("TurnAround")
				elseif j == 8 then
					cardactioncomp:SetText("Forward")
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
				
		local posX, posY, dirX, dirY = _posX, _posY, _dirX, _dirY
				
		for i = 1, self.NumberOfCardsToPick do
			
			-- Get target.
			local targetCheckpoint = world:GetComponent(_unitID, "TargetCheckpoint", 0):GetInt()
			local checkpoints = self:GetEntities("Checkpoint")
			local targetX, targetY = self:GetTargetPosition(checkpoints, targetCheckpoint)
			
			local simFellDown, simPosX, simPosY, simDirX, simDirY
			local cardsToSim = {}
			cardsToSim.__mode = "k"
			
			local bestCardId, bestDist, bestNextDist
			local dist, nextDist, prevDist
						
			bestCardId = 0
			
			bestNextDist, bestDist = 1000000, 1000000
			
			for i = 1, #CardSetAI do
				table.insert(cardsToSim, CardSetAI[i])

				-- Simulate playing the ith card.
				simFellDown, simPosX, simPosY, simDirX, simDirY = self:SimulateCardsFromPos(_unitID, posX, posY, dirX, dirY, cardsToSim)
				
				dist = PathfinderHandler.GeneratePath(simPosX, simPosY, targetX, targetY)
				
				if simFellDown then
					local playedCards = #pickedCards
					local extraCost = self.NumberOfCardsToPick - playedCards - 1
					dist = dist + extraCost
				end
				
				if dist <= bestDist then
				
					-- Get the distance from a cell as if we have walked forward.
					nextDist = PathfinderHandler.GeneratePath(simPosX + simDirX, simPosY + simDirY, _targetX, _targetY)
					
					-- Get the distance from a cell as if we have walked backward and compare it to the previous.
					nextDist = math.min(nextDist, PathfinderHandler.GeneratePath(simPosX - simDirX, simPosY - simDirY, _targetX, _targetY))
					
					if simFellDown then
						local playedCards = #pickedCards
						local extraCost = self.NumberOfCardsToPick - playedCards - 1
						nextDist = nextDist + extraCost
					end
				
					if dist == bestDist then
						
						if nextDist < bestNextDist then
							
							bestCardId = i
							bestNextDist = nextDist
						end
					else
						
						bestCardId = i
						bestNextDist = nextDist
						bestDist = dist
					end
				end
				
				table.remove(cardsToSim, 1)
			end
			
			-- Choose card
			table.insert(pickedCards, CardSetAI[bestCardId])
			table.insert(cardsToSim, CardSetAI[bestCardId])
			simFellDown, posX, posY, dirX, dirY = self:SimulateCardsFromPos(_unitID, posX, posY, dirX, dirY, cardsToSim)
			table.remove(cardsToSim, 1)
			table.remove(CardSetAI, bestCardId)
		end
	end
	return pickedCards
end

AICardPickingSystem.GetAllCardsOf = function( self, CardSetAI, cardName )
	
	local cards = {}
	cards.__mode = "k"
	for i = 1, #CardSetAI do

		local nameCard = world:GetComponent(CardSetAI[i], "CardAction", 0):GetText()

		if cardName == nameCard then
			cards[#cards + 1] = CardSetAI[i]
		end
	end
	return cards
end

AICardPickingSystem.SendCards = function(self, _player, _pickedcards)
	
	--print("AI Send Cards")
	local unit = world:GetComponent(_player, "UnitEntityId", "Id"):GetInt()
	
	world:CreateComponentAndAddTo("HasSelectedCards", _player)
	world:CreateComponentAndAddTo("UnitSelectedCards", unit)

	for i = 1, self.NumberOfCardsToPick do
		local action = world:GetComponent(_pickedcards[i], "CardAction", 0):GetText()
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
		
	return self:SimulateCardsFromPos(_unit, posX, posY, dirX, dirY, _pickedcards)
end

AICardPickingSystem.SimulateCardsFromPos = function(self, _unit, _posX, _posY, _dirX, _dirY, _pickedcards)
	
	local posX, posY, dirX, dirY = _posX, _posY, _dirX, _dirY
	
	local fellDown = false
	
	if self.PrintSimulation == 1 then
		print()
		print("----------------- NEW SIMULATION STARTED --------------------")
	end
	
	local cardName
	
	for i = 1, #_pickedcards do
		
		if type(_pickedcards[i]) == "string" then
			cardName = _pickedcards[i]
		else
			cardName = world:GetComponent(_pickedcards[i], "CardAction", 0):GetText()
		end
		
		if self.PrintSimulation == 1 then
			print()
			print("Card number:", i, "is", cardName)
			print()
		end
		
		if cardName == "Forward" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 1, false)
			
		elseif cardName == "Backward" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, false, false, 1, false)
			
		elseif cardName == "TurnLeft" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 1)
			
		elseif cardName == "TurnRight" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 3)
			
		elseif cardName == "TurnAround" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 2)
			
		elseif cardName == "Dodge" or cardName == "SlingShot" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 0)
			
		elseif cardName == "Sprint" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 2, false)
			
		--elseif cardName == "Leap" then
		--	
		--	fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, true, 2, false)
			
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
	
	return fellDown, posX, posY, dirX, dirY
end

AICardPickingSystem.SimulateMoveForward = function(self, _posX, _posY, _dirX, _dirY, _forwards, _jump, _iterations, _riverMove)
	
	local forward = -1
	if _forwards then
		forward = 1
	end
	
	local fellDown = false
	local posX = _posX
	local posY = _posY
				
	for i = 1, _iterations do
		
		posX = posX + _dirX * forward
		posY = posY + _dirY * forward
		
		if self:TileHasComponent("NotWalkable", posX, posY) then
			
			posX = posX - _dirX * forward
			posY = posY - _dirY * forward
			
		elseif self:TileHasComponent("Void", posX, posY) and not _jump then
			
			fellDown = true
			if self.PrintSimulation == 1 then
				print("I will fall down in", posX, posY)
			end
			break
		end
				
		if not _riverMove and i == _iterations and self:TileHasComponent("River", posX, posY) then
			
			local waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(posX, posY)
			
			for j = 1, waterSpeed do
			
				if self:TileHasComponent("River", posX + waterDirX, posY + waterDirY) then
					posX = posX + waterDirX
					posY = posY + waterDirY
					waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(posX, posY)
				else
					fellDown, posX, posY = self:SimulateMoveForward(posX, posY, waterDirX, waterDirY, true, false, 1, true)
					break
				end
				
				if self.PrintSimulation == 1 then
					print("I will move in river with X, Y, speed:", waterDirX, waterDirY, waterSpeed)
				end
			end
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
		
		fellDown, posX, posY = self:SimulateMoveForward(_posX, _posY, waterDirX, waterDirY, true, false, waterSpeed, true)
		
		if self.PrintSimulation == 1 then
			print("Pos", posX, posY, "Dir", dirX, dirY, "with", _iterations)
		end
	elseif self.PrintSimulation == 1 then
		print("Dir", dirX, dirY, "with", _iterations, "turn left iterations")
	end
	
	return fellDown, posX, posY, dirX, dirY
end

AICardPickingSystem.TileHasComponent = function(self, _component, _posX, _posY)
	
	local mapSize = self:GetEntities("MapSpecs")
	local mapSizeComp = world:GetComponent(mapSize[1], "MapSpecs", "SizeX")
	local mapX, mapY = mapSizeComp:GetInt2()
	local tiles = self:GetEntities("TileComp")
	local returnValue
		
	if -1 < _posX and _posX < mapX and -1 < _posY and _posY < mapY then
		returnValue = world:EntityHasComponent(tiles[mapX * _posY + _posX + 1], _component)
	else
		print("ERROR, trying to get entity from tile outside the boundaries")
		returnValue = false
	end
	
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
			local card = world:GetComponent(entities[i], "CardAction", 0):GetText()
		end
	end
end