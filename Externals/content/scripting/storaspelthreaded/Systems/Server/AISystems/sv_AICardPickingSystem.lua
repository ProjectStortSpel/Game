AICardPickingSystem = System()
--AICardPickingSystem.PrintSimulation = 0
--AICardPickingSystem.PrintThis = false
AICardPickingSystem.ERROR = false -- TODO: Remove this

AICardPickingSystem.AICheat = false
AICardPickingSystem.TimeLimitPerUpdateInMs = 10.0

-- Flags to indicate if init is done and when to pick cards.
AICardPickingSystem.InitDone = false
AICardPickingSystem.TimeToPick = false

-- Dealing settings (do not set here)
AICardPickingSystem.CardsPerHand = 0
AICardPickingSystem.CardsToPick = 0

-- Map variables (do not set here)
AICardPickingSystem.MapSizeX = 0
AICardPickingSystem.MapSizeY = 0
AICardPickingSystem.NoOfCheckpoints = 0
AICardPickingSystem.Map = {}
AICardPickingSystem.Map.__mode = "k"
AICardPickingSystem.RiverDirections = {}
AICardPickingSystem.RiverDirections.__mode = "k"
AICardPickingSystem.CheckpointPositions = {}
AICardPickingSystem.CheckpointPositions.__mode = "k"
AICardPickingSystem.TempStones = {}
AICardPickingSystem.TempStones.__mode = "k"

-- Simulation variables (do not set here)
AICardPickingSystem.CurrentAI = 1
AICardPickingSystem.PermutationIndex = 1
AICardPickingSystem.PermutationsArray = ''
AICardPickingSystem.SimStones = {}
AICardPickingSystem.SimStones.__mode = "k"
AICardPickingSystem.StoneLifeSpanSet = false
AICardPickingSystem.StoneLifeSpan = 0

-- Per AI
AICardPickingSystem.CardSet = {}
AICardPickingSystem.CardSet.__mode = "k"

-- Per AI (actually per spawnpoint)
AICardPickingSystem.Positions = {}
AICardPickingSystem.Positions.__mode = "k"
AICardPickingSystem.Directions = {}
AICardPickingSystem.Directions.__mode = "k"
AICardPickingSystem.TargetCheckpoints = {}
AICardPickingSystem.TargetCheckpoints.__mode = "k"
AICardPickingSystem.Spawnpoints = {}
AICardPickingSystem.Spawnpoints.__mode = "k"
AICardPickingSystem.UnitEntityIds = {}
AICardPickingSystem.UnitEntityIds.__mode = "k"
AICardPickingSystem.SimulationGhosts = {}
AICardPickingSystem.SimulationGhosts.__mode = "k"

AICardPickingSystem.ChosenCardsArray = {}
AICardPickingSystem.ChosenCardsArray.__mode = "k"
AICardPickingSystem.BestDistances = {}
AICardPickingSystem.BestDistances.__mode = "k"

-- TODO: Remove this and the timer in combinationmath.
AICardPickingSystem.Sleep = function(_milliseconds)
	local t0 = os.clock()
	while (os.clock() - t0) * 1000 < _milliseconds do end
end

AICardPickingSystem.Initialize = function(self)
	self:SetName("AI card picking System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingUpdate()
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AICard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealingSettings", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AutoPickCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileWalkabilityHasChanged", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("HasSelectedCards", FilterType.Excluded)
end

AICardPickingSystem.Update = function(self, dt)
	
	if not self.TimeToPick then
		return
	end
	
	local startTime, endTime
	local timeTaken = 0.0
	
	local timeLimit = self.TimeLimitPerUpdateInMs
	local cardsToPick = self.CardsToPick
	
	startTime = os.clock()
	
	local aiIndex = self.CurrentAI
	local AIs = self:GetEntities("AI")
	local noOfAIs = #AIs
	
	while timeTaken < timeLimit do
		
		if noOfAIs < aiIndex then
			
			local permutationIndex = self.PermutationIndex + cardsToPick
			self.PermutationIndex = permutationIndex
			
			if permutationIndex > #self.PermutationsArray then
				print("AI done with all permutations")
				self:SendCards()
				return
			end
			
			aiIndex = 1
		end
		
		local playerNumber = world:GetComponent(AIs[aiIndex], "PlayerNumber", 0):GetInt(0)
		
		PotentialFieldHandler.UseMyPF(playerNumber)
		self:PickCards(playerNumber)
		
		endTime = os.clock()
		timeTaken = (endTime - startTime) * 1000
		
		aiIndex = aiIndex + 1
	end
	
	self.CurrentAI = aiIndex
end

AICardPickingSystem.GetAIsCardSet = function(self, _AI, _cards)

		local aisCard = {}
		aisCard.__mode = "k"
		local aiNr = world:GetComponent(_AI, "PlayerNumber", 0):GetInt()
		for j = 1, #_cards do
			local card = world:GetComponent(_cards[j], "DealtCard", 0)
			local id = card:GetInt()
			local plyNr = world:GetComponent(id, "PlayerNumber", 0):GetInt()

			if plyNr == aiNr then
				
				aisCard[#aisCard+1] = _cards[j]
				--print(_AI, j)
			end
		end
		
		return aisCard
end

AICardPickingSystem.PickCards = function( self, _playerNumber )
	
	local startTime = 0
	local endTime = 0
	local timeTaken1 = 0
	local timeTaken2 = 0
	local timeTaken3 = 0
	local timeTaken4 = 0
	local timeTaken5 = 0
	local timeTaken6 = 0
	local timeTaken7 = 0
	local timeTaken8 = 0
	local timeTaken9 = 0
	
	local cardsToSim = {}
	cardsToSim.__mode = "k"
	
--startTime = CombinationMath.ClockCycles()
	
	local targetNr = self.TargetCheckpoints[_playerNumber]
	--world:GetComponent(_unitID, "TargetCheckpoint", 0):GetInt(0)
	
	local targetX, targetY = self:GetCheckpointPos(targetNr)
	local cardsPerHand = self.CardsPerHand
	local cardsToPick = self.CardsToPick
	
--endTime = CombinationMath.ClockCycles()
--timeTaken1 = endTime - startTime
	
--startTime = CombinationMath.ClockCycles()
	
	-- Get the cards to simulate by accessing the permutations array. CardsToSim will contain strings representing the actions of the cards.
	for cardIndex = 1, cardsToPick do
		
		local charVar = string.byte(self.PermutationsArray, self.PermutationIndex + cardIndex - 1)
		
		local arrayIndex = (_playerNumber - 1) * cardsPerHand + charVar
		cardsToSim[cardIndex] = self.CardSet[arrayIndex]
		
		if self.AICheat then
			--io.write(cardsToSim[cardIndex], ", ")
			cardsToSim[cardIndex] = self:ChangeCardActionString(charVar)
			--io.write(charVar, " ", cardsToSim[cardIndex], ", ")
		end
	end
	--io.write("\n")
	
--endTime = CombinationMath.ClockCycles()
--timeTaken2 = (endTime - startTime)
	
	
--startTime = CombinationMath.ClockCycles()
	-- Get the distance from where we end up using the cards.
	local simFellDown, simCPsReached, simPosX, simPosY, simDirX, simDirY = self:SimulateCards(_playerNumber, targetNr, cardsToSim)
	
	--startTime = CombinationMath.ClockCycles()
	--local simFellDown, simCPsReached, simPosX, simPosY, simDirX, simDirY = self:OldSimulateCards(_playerNumber, targetNr, cardsToSim)
	--endTime = CombinationMath.ClockCycles()
	--timeTaken3 = (endTime - startTime)
--endTime = CombinationMath.ClockCycles()
--timeTaken3 = (endTime - startTime)
	
	--io.write("New: ", timeTaken2, " Old: ", timeTaken3, "\n")
	
--startTime = CombinationMath.ClockCycles()
	
	-- If a checkpoint has been reached, change the target.
	if 0 < simCPsReached then
		
		--print("checkpoint reached in the middle of a round")
		--io.write("target before: ", targetX, ", ", targetY, ". SimCPsReached: ", simCPsReached, "\n")
		--print("checkpoint reached", targetNr, simCPsReached)
		targetNr = targetNr + simCPsReached
		
		if targetNr < self.NoOfCheckpoints then
			
			targetX, targetY = self:GetCheckpointPos(targetNr)
		else
			targetX, targetY = self:GetCheckpointPos(self.NoOfCheckpoints)
		end
		--print("target", targetX, targetY)
		
		--io.write("target after: ", targetX, ", ", targetY, "\n")
	end
	
--endTime = CombinationMath.ClockCycles()
--timeTaken4 = (endTime - startTime)
	
	
--startTime = CombinationMath.ClockCycles()
	local dist = PathfinderHandler.GeneratePath(simPosX, simPosY, targetX, targetY)
	
--endTime = CombinationMath.ClockCycles()
--timeTaken5 = (endTime - startTime)
	
	-- TODO: Add method to vaska cards if we will fall down.
	--if fellDown then
	--	Vaska kort!
	--end
	-- TODO: Only do nextDist(forward) if have forward cards, only do nextDist(backward) if have backward cards.
	-- TODO: Don't use walk-cards if we end up at the same spot.
	
--startTime = CombinationMath.ClockCycles()
	local bestDist = self.BestDistances[_playerNumber * 3 - 2]
	local bestCPsReached = self.BestDistances[_playerNumber * 3]
	
--endTime = CombinationMath.ClockCycles()
--timeTaken6 = (endTime - startTime)
	
	local newRound = false
	if self.PermutationIndex == 1 then
		newRound = true
	end
	
	-- If this is the first update on a new round, or if more checkpoints was reached, or if the distance is better than the previously best solution found, then get the nextDist.
	if newRound 
	or bestCPsReached < simCPsReached 
	or (bestCPsReached == simCPsReached and dist <= bestDist) then
		
--startTime = CombinationMath.ClockCycles()
		-- Get the distance from a cell one tile ahead, then get distance from one tile behind and compare the two. Save the best.
		local nextDist = PathfinderHandler.GeneratePath(simPosX + simDirX, simPosY + simDirY, targetX, targetY)
		nextDist = math.min(nextDist, PathfinderHandler.GeneratePath(simPosX - simDirX, simPosY - simDirY, targetX, targetY))
		
		local bestNextDist = self.BestDistances[_playerNumber * 3 - 1]
	
--endTime = CombinationMath.ClockCycles()
--timeTaken7 = (endTime - startTime)
		
		-- Update the best solution only if the dist is better or if nextDist is better than nextDist (and dist is equal to bestDist).
		if newRound 
		or bestCPsReached < simCPsReached
		or dist < bestDist 
		or nextDist < bestNextDist then
			
--startTime = CombinationMath.ClockCycles()
			
			self.BestDistances[_playerNumber * 3 - 2] = dist
			self.BestDistances[_playerNumber * 3 - 1] = nextDist
			self.BestDistances[_playerNumber * 3] = simCPsReached
	
--endTime = CombinationMath.ClockCycles()
--timeTaken8 = (endTime - startTime)
			
--startTime = CombinationMath.ClockCycles()
			--io.write("\nPlayer ", _playerNumber, "\n")
			-- Save the cards which where used.
			for card = 1, cardsToPick do
				
				local charVar = string.byte(self.PermutationsArray, self.PermutationIndex + card - 1)
				local arrayIndex = (_playerNumber - 1) * cardsToPick + card
				
				self.ChosenCardsArray[arrayIndex] = charVar
				
				local	finalRotation	=	self:GetGhostRotation(simDirX, simDirY)
				world:GetComponent(self.SimulationGhosts[_playerNumber], "Position", "X"):SetFloat3(simPosX, 0.5, simPosY)
				--world:GetComponent(self.SimulationGhost, "Direction", "X"):SetInt2(dirX, dirY)
				world:GetComponent(self.SimulationGhosts[_playerNumber], "Rotation", "Y"):SetFloat(finalRotation)
				
				--self.ChosenCardsArray[arrayIndex] = cardsToSim[card]
				--io.write("Id: ", charVar, " ", cardsToSim[card], ", ")
			end
	
--endTime = CombinationMath.ClockCycles()
--timeTaken9 = (endTime - startTime)
			--io.write("\n SimCPsReached: ", simCPsReached, ", dist: ", dist, ", nextDist: ", nextDist, "\n")
		end
	end
	
	--if timeTaken9 ~= 0 then print(timeTaken1, "Get targets") end
	--if timeTaken9 ~= 0 then print(timeTaken2, "Get cards") end
	--if timeTaken9 ~= 0 then print(timeTaken3, "Simulate") end
	--if timeTaken9 ~= 0 then print(timeTaken4, "SimCPsReached") end
	--if timeTaken9 ~= 0 then print(timeTaken5, "Pathfinder") end
	--if timeTaken9 ~= 0 then print(timeTaken6, "GetBestDistances") end
	--if timeTaken9 ~= 0 then print(timeTaken7, "GetNextDist (Pathfinder)") end
	--if timeTaken9 ~= 0 then print(timeTaken8, "Save best distances") end
	--if timeTaken9 ~= 0 then print(timeTaken9, "Saving cards") print() end
	
	--self.Sleep(100)
	
end

AICardPickingSystem.SendCards = function(self)
	
	self.TimeToPick = false
	local AIs = self:GetEntities("AI")
	
	for ai = 1, #AIs do
		
		playerEntity = AIs[ai]
		local unit = world:GetComponent(playerEntity, "UnitEntityId", "Id"):GetInt()
		
		world:CreateComponentAndAddTo("HasSelectedCards", playerEntity)
		--world:CreateComponentAndAddTo("UnitSelectedCards", unit)
		
		local DealingSettings = self:GetEntities("DealingSettings")
		local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
		
		local cardSet = self:GetAIsCardSet(playerEntity, self:GetEntities("AICard"))
		local playerNumber = world:GetComponent(playerEntity, "PlayerNumber", 0):GetInt(0)

		for card = 1, cardsToPick do
			
			local arrayIndex = (playerNumber - 1) * cardsToPick + card
			local cardIndex = self.ChosenCardsArray[arrayIndex]
			
			if self.AICheat then
				self:ChangeCardInSet(cardIndex, cardSet)
			end
			
			local action = world:GetComponent(cardSet[cardIndex], "CardAction", 0):GetText()
			--local prio = world:GetComponent(cardSet[cardIndex], "CardPrio", 0):GetInt()
			--print("AI Action: " .. action .. " - Prio: " .. prio)
			--io.write(cardIndex, " ", action, ", ")
			
			world:RemoveComponentFrom("DealtCard", cardSet[cardIndex])
			world:RemoveComponentFrom("AICard", cardSet[cardIndex])
			world:CreateComponentAndAddTo("CardStep", cardSet[cardIndex])
			world:SetComponent(cardSet[cardIndex], "CardStep", "Step", card)
			world:SetComponent(cardSet[cardIndex], "CardStep", "UnitEntityId", unit)
		end
		--io.write("\n")

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("NotifyStartNewRound", id)
		world:GetComponent(id, "NotifyStartNewRound", "IsAI"):SetBool(true) 
	end
	--print("AI sent cards")
end

AICardPickingSystem.SimulateCards = function(self, _playerNumber, _targetCheckpointNumber, _pickedcards)
	
	
--local startTime = 0
--local endTime = 0
--local timeTaken1 = 0
--local timeTaken2 = 0
--local timeTaken3 = 0
--local timeTaken4 = 0
--local timeTaken5 = 0
--local timeTaken6 = 0
--local timeTaken7 = 0
--local timeTaken8 = 0
--local timeTaken9 = 0

--self.Sleep(1000)
	
	local simStoneAdded = false
	
	--startTime = CombinationMath.ClockCycles()
	
	--local posX, posY = world:GetComponent(_unit, "MapPosition", 0):GetInt2(0)
	--local dirX, dirY = world:GetComponent(_unit, "Direction", 0):GetInt2(0)
	local doubleIndex = (_playerNumber - 1) * 2 + 1 
	
	local posX = self.Positions[doubleIndex]
	local posY = self.Positions[doubleIndex + 1]
	local dirX = self.Directions[doubleIndex]
	local dirY = self.Directions[doubleIndex + 1]
	
--print("\nBefore simulation: ", posX, posY, dirX, dirY, " Picked Cards: ", #_pickedcards)
	
	--self.Sleep(100)
	
	local fellDown = false
	
	--if self.PrintSimulation == 1 then
	--	print()
	--	print("----------------- NEW SIMULATION STARTED --------------------")
	--end
	
	local cardName = ""
	local checkPointsReached = 0
	local targetCheckpointNumber = _targetCheckpointNumber
	
	
	
	--local printThis = false
	
	
	for n = 1, #_pickedcards do
		
--		startTime = CombinationMath.ClockCycles()
		
		cardName = _pickedcards[n]
	
--		endTime = CombinationMath.ClockCycles()
--		timeTaken1 = (endTime - startTime)
	
	--self.Sleep(100)
		
		--if self.PrintSimulation == 1 then
		--	print()
		--	print("Card number:", n, "is", cardName)
		--	print()
		--end
		
		--if n == 1 then
		--	if _pickedcards[1] == "Stone" and _pickedcards[2] == "Stone" and _pickedcards[3] == "Backward" then
		--		--print("This is the shit")
		--		--printThis = true
		--		--print(posX, posY, #_pickedcards)
		--		self.PrintThis = true
		--	else
		--		self.PrintThis = false
		--	end
		--	
		--end
		
		
		if cardName == "Forward" then
			
			--startTime = CombinationMath.ClockCycles()
			
			--fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 1, false)
			fellDown, posX, posY = self:SimulateForward(posX, posY, dirX, dirY, n)
			
--print("After forward:   ", posX, posY, dirX, dirY)
			--endTime = CombinationMath.ClockCycles()
			--timeTaken2 = (endTime - startTime)
	
	--self.Sleep(100)
			
		elseif cardName == "Backward" then
			
			--startTime = CombinationMath.ClockCycles()
			
			--fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, false, false, 1, false)
			fellDown, posX, posY = self:SimulateForward(posX, posY, -dirX, -dirY, n)
--print("After backward: ", posX, posY, dirX, dirY)
			
			--endTime = CombinationMath.ClockCycles()
			--timeTaken3 = (endTime - startTime)
	
	--self.Sleep(100)
			
		elseif cardName == "TurnLeft" then
			
			--startTime = CombinationMath.ClockCycles()
		
			--fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 1)
			fellDown, posX, posY, dirX, dirY = self:SimulateLeft(posX, posY, dirX, dirY, n)
--print("After turn left: ", posX, posY, dirX, dirY)
			
			--endTime = CombinationMath.ClockCycles()
			--timeTaken4 = (endTime - startTime)
	
	--self.Sleep(100)
			
		elseif cardName == "TurnRight" then
			
			--startTime = CombinationMath.ClockCycles()
		
			--fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 3)
			fellDown, posX, posY, dirX, dirY = self:SimulateRight(posX, posY, dirX, dirY, n)
--print("After turn right: ", posX, posY, dirX, dirY)
			
			--endTime = CombinationMath.ClockCycles()
			--timeTaken5 = (endTime - startTime)
	
	--self.Sleep(100)
			
		elseif cardName == "TurnAround" then
			
			--startTime = CombinationMath.ClockCycles()
		
			--fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 2)
			if self.Map[arrayIndex] == "River" then
				fellDown, posX, posY = self:SimulateRiverMove(posX, posY, n)
			end
			
			dirX = -dirX
			dirY = -dirY
			
--print("After turn around: ", posX, posY, dirX, dirY)
			--endTime = CombinationMath.ClockCycles()
			--timeTaken6 = (endTime - startTime)
	
	--self.Sleep(100)
			
		elseif cardName == "Guard" or cardName == "SlingShot" then
			
			--startTime = CombinationMath.ClockCycles()
		
			--fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 0)
			if self.Map[arrayIndex] == "River" then
				fellDown, posX, posY = self:SimulateRiverMove(posX, posY, n)
			end
			
--print("After Stand still: ", posX, posY, dirX, dirY)
			--endTime = CombinationMath.ClockCycles()
			--timeTaken7 = (endTime - startTime)
	
	--self.Sleep(100)
			
		elseif cardName == "Sprint" then
			
			--startTime = CombinationMath.ClockCycles()
		
			--fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 2, false)
			fellDown, posX, posY = self:SimulateSprint(posX, posY, dirX, dirY, n)
			
--print("After Sprint: ", posX, posY, dirX, dirY)
			--endTime = CombinationMath.ClockCycles()
			--timeTaken8 = (endTime - startTime)
	
	--self.Sleep(100)
			
		elseif cardName == "Stone" then
			
			--fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 1, false)
			fellDown, posX, posY = self:SimulateStone(posX, posY, dirX, dirY, n)
			simStoneAdded = true
			
--print("After Stone:     ", posX, posY, dirX, dirY)
		else
		
			print("Warning: Card Not Added In SimulateCards (AI)", cardName)
		end
		
		if fellDown then
			
			--posX, posY = world:GetComponent(_unit, "Spawnpoint", 0):GetInt2()
--print("Felldown")
			posX = self.Spawnpoints[doubleIndex]
			posY = self.Spawnpoints[doubleIndex + 1]
			break
		end
		
		--startTime = CombinationMath.ClockCycles()
		
		if self:ReachedCheckpoint(targetCheckpointNumber, posX, posY) then
			
			checkPointsReached = checkPointsReached + 1
			targetCheckpointNumber = targetCheckpointNumber + 1
			
			-- Hax. If it reached the last checkpoint, it is preferable to go there with as few cards as possible.
			if self.NoOfCheckpoints < targetCheckpointNumber  then
				
				checkPointsReached = checkPointsReached + #_pickedcards - n
				--print("Reaching last checkpoint. Adding ", #_pickedcards - n, " to checkPointsReached")
				targetCheckpointNumber = targetCheckpointNumber - 1
			end
			
		end
		
		--if printThis then
		--	print(posX, posY)
		--end
			
		--endTime = CombinationMath.ClockCycles()
		--timeTaken9 = (endTime - startTime)
	end
	
	if simStoneAdded then
		local mapX = self.MapSizeX
		local mapY = self.MapSizeY
		for y = 0, mapY - 1 do
			for x = 0, mapX - 1 do
				self.SimStones[mapX * y + x + 1] = 0
			end
		end
	end
	
	--if self.PrintSimulation == 1 then
		--print("I will end up at:", posX, posY, "with dir:", dirX, dirY)
	--end
	
	--if timeTaken1 ~= 0 then print(timeTaken1, "Get Card name") end
	--if timeTaken2 ~= 0 then print(timeTaken2, "Forward") end
	--if timeTaken3 ~= 0 then print(timeTaken3, "Backward") end
	--if timeTaken4 ~= 0 then print(timeTaken4, "TurnLeft") end
	--if timeTaken5 ~= 0 then print(timeTaken5, "TurnRight") end
	--if timeTaken6 ~= 0 then print(timeTaken6, "TurnAround") end
	--if timeTaken7 ~= 0 then print(timeTaken7, "Guard/SlingShot") end
	--if timeTaken8 ~= 0 then print(timeTaken8, "Sprint") end
	--if timeTaken9 ~= 0 then print(timeTaken9, "Reached Checkpoint") end
	--
	--self.Sleep(100)
	
	
	--if _pickedcards[1] == "Stone" and _pickedcards[2] == "Stone" and _pickedcards[3] == "Backward" then
	
		--local	finalRotation	=	self:GetGhostRotation(dirX, dirY)
		--world:GetComponent(self.SimulationGhost, "Position", "X"):SetFloat3(posX, 0.5, posY)
		--world:GetComponent(self.SimulationGhost, "Direction", "X"):SetInt2(dirX, dirY)
		--world:GetComponent(self.SimulationGhost, "Rotation", "Y"):SetFloat(finalRotation)
		--world:GetComponent(self.SimulationGhost, "Color", "X"):SetFloat3(1, 1, 1)
		
	--end
	
	
	
	return fellDown, checkPointsReached, posX, posY, dirX, dirY
end

AICardPickingSystem.GetCheckpointPos = function(self, _targetCheckpointNumber)
	
	--print("getcheckpointpos")
	local firstIndex = (_targetCheckpointNumber - 1) * 2 + 1
	local posX = self.CheckpointPositions[firstIndex]
	local posY = self.CheckpointPositions[firstIndex + 1]
	--print("getcheckpointpos done")
	return posX, posY
end

AICardPickingSystem.ReachedCheckpoint = function(self, _targetCheckpointNumber, _posX, _posY)
	
	--print("ReachedCheckpoint")
	local startIndex = (_targetCheckpointNumber - 1) * 2 + 1
	
	local cpPosX = self.CheckpointPositions[startIndex]
	local cpPosY = self.CheckpointPositions[startIndex + 1]
	
	--print("ReachedCheckpoint done")
	if _posX == cpPosX and _posY == cpPosY then
		return true
	end
	
	return false
end

AICardPickingSystem.SimulateForward = function(self, _posX, _posY, _dirX, _dirY, _step)
	
	local fellDown = false
	local posX = _posX
	local posY = _posY
	
	posX = posX + _dirX
	posY = posY + _dirY
	
	local arrayIndex = self.MapSizeX * posY + posX + 1
	
	if self.Map[arrayIndex] == "Void" then
		fellDown = true
	else
		--if self.SimStones[arrayIndex] > 0 then
		--	print(_step, self.SimStones[arrayIndex])
		--end
		--if self.PrintThis then
		--
		--	io.write("\n\n")
		--	for stoneY = 0, self.MapSizeY - 1 do
		--		for stoneX = 0, self.MapSizeX - 1 do
		--			io.write(self.SimStones[self.MapSizeX * stoneY + stoneX + 1])
		--		end
		--		io.write("\n")
		--	end
		--	
		--	print(_step, self.SimStones[arrayIndex])
		--end
		if self.Map[arrayIndex] == "NotWalkable" or _step < self.TempStones[arrayIndex] or _step <= self.SimStones[arrayIndex] then
			--if _step <= self.SimStones[arrayIndex] then
			--	print("Sim stone in the way")
			--end
			posX = _posX
			posY = _posY
			-- Change index to check if river where we currently are.
			arrayIndex = self.MapSizeX * posY + posX + 1
		end
		
		if self.Map[arrayIndex] == "River" then
			fellDown, posX, posY = self:SimulateRiverMove(posX, posY, _step)
		end
	end
	
	return fellDown, posX, posY
end

AICardPickingSystem.SimulateRiverMove = function(self, _posX, _posY, _step)
	
	local fellDown = false
	local posX = _posX
	local posY = _posY
	
	local riverDirX, riverDirY = self:GetLocalRiverVariables(posX, posY)
	
	--print("River at 	", posX, posY, " with dir ", riverDirX, riverDirY)
	posX = posX + riverDirX
	posY = posY + riverDirY
	
	local arrayIndex = self.MapSizeX * posY + posX + 1
	
	if self.Map[arrayIndex] == "Void" then
		fellDown = true
		--print("Fell down because of river")
	elseif self.Map[arrayIndex] == "NotWalkable" or _step < self.TempStones[arrayIndex] or _step <= self.SimStones[arrayIndex] then
		posX = _posX
		posY = _posY
		--print()
		--print("Cannot riverpush, stone in the way")
		--print()
	end
	
	return fellDown, posX, posY
end

AICardPickingSystem.SimulateLeft = function(self, _posX, _posY, _dirX, _dirY, _step)
	
	local fellDown = false
	local posX, posY = _posX, _posY
	
	local dirX = _dirY
	local dirY = -_dirX
	
	if self.Map[self.MapSizeX * posY + posX + 1] == "River" then
		fellDown, posX, posY = self:SimulateRiverMove(posX, posY, _step)
	end
	
	return fellDown, posX, posY, dirX, dirY
end

AICardPickingSystem.SimulateRight = function(self, _posX, _posY, _dirX, _dirY, _step)
	
	local fellDown = false
	local posX, posY = _posX, _posY
	
	local dirX = -_dirY
	local dirY = _dirX
	
	if self.Map[self.MapSizeX * posY + posX + 1] == "River" then
		fellDown, posX, posY = self:SimulateRiverMove(posX, posY, _step)
	end
	
	return fellDown, posX, posY, dirX, dirY
end

AICardPickingSystem.SimulateSprint = function(self, _posX, _posY, _dirX, _dirY, _step)
	
	local fellDown = false
	local posX = _posX
	local posY = _posY
	
	posX = posX + _dirX
	posY = posY + _dirY
	
	local arrayIndex = self.MapSizeX * posY + posX + 1
	
	-- The first step is different from regular forward as we do not need to check another step if it falls down or is blocked by a not walkable. If it is blocked, check for river.
	if self.Map[arrayIndex] == "Void" then
		return true, _posX, _posY
	elseif self.Map[arrayIndex] == "NotWalkable" or _step < self.TempStones[arrayIndex] or _step <= self.SimStones[arrayIndex] then
		posX = _posX
		posY = _posY
		-- Change index to check if river where we currently are.
		arrayIndex = self.MapSizeX * posY + posX + 1
		
		if self.Map[arrayIndex] == "River" then
			fellDown, posX, posY = self:SimulateRiverMove(posX, posY, _step)
		end
		
		return fellDown, posX, posY
	end
	
	-- The last "step" of the sprint works like a forward.
	return self:SimulateForward(posX, posY, _dirX, _dirY, _step)
end

AICardPickingSystem.SimulateStone = function(self, _posX, _posY, _dirX, _dirY, _step)
	
	local fellDown = false
	local posX = _posX
	local posY = _posY
	local layStone = true
	
	posX = posX + _dirX
	posY = posY + _dirY
	
	local arrayIndex = self.MapSizeX * posY + posX + 1
	
	if self.Map[arrayIndex] == "Void" then
		fellDown = true
	else
		if self.Map[arrayIndex] == "NotWalkable" or _step < self.TempStones[arrayIndex] or _step <= self.SimStones[arrayIndex] then
			posX = _posX
			posY = _posY
			-- Change index to check if river where we currently are.
			arrayIndex = self.MapSizeX * posY + posX + 1
			layStone = false
		end
		
		if self.Map[arrayIndex] == "River" then
			fellDown, posX, posY = self:SimulateRiverMove(posX, posY, _step)
		end
	end
	
	if layStone then
		--print("adding sim stone", _posX, _posY)
		self.SimStones[self.MapSizeX * _posY + _posX + 1] = self.StoneLifeSpan + _step - 1
	end
	
	return fellDown, posX, posY
end

AICardPickingSystem.GetLocalRiverVariables = function(self, _posX, _posY)
	
	local mapX = self.MapSizeX
	
	local dirX = self.RiverDirections[(mapX * _posY + _posX) * 2 + 1]
	local dirY = self.RiverDirections[(mapX * _posY + _posX) * 2 + 2]
	
	return dirX, dirY
end

AICardPickingSystem.GetRiverVariables = function(self, _posX, _posY)
	
	local mapSizeEntities = self:GetEntities("MapSpecs")
	local mapX = world:GetComponent(mapSizeEntities[1], "MapSpecs", "SizeX"):GetInt()
	local tiles = self:GetEntities("TileComp")
	local dirX, dirY, speed = world:GetComponent(tiles[mapX * _posY + _posX + 1], "River", 0):GetInt3(0)
	
	return dirX, dirY, speed
end

AICardPickingSystem.ChangeCardInSet = function(self, _cardIndex, _cardSet)
	
	local cardactioncomp = world:GetComponent(_cardSet[_cardIndex], "CardAction", 0)
	
	cardactioncomp:SetText(self:ChangeCardActionString(_cardIndex))
end

AICardPickingSystem.ChangeCardActionString = function(self, _cardIndex)
	
	local action = "Forward"
	
		if _cardIndex ==  1 then action = ("TurnLeft")
	elseif _cardIndex ==  2 then action = ("TurnLeft")
	elseif _cardIndex ==  3 then action = ("Backward")
	elseif _cardIndex ==  4 then action = ("Backward")
	elseif _cardIndex ==  5 then action = ("Backward")
	elseif _cardIndex ==  6 then action = ("Backward")
	elseif _cardIndex ==  7 then action = ("Stone")
	elseif _cardIndex ==  8 then action = ("Stone")
	elseif _cardIndex ==  9 then action = ("Forward")
	elseif _cardIndex == 10 then action = ("Forward")
	elseif _cardIndex == 11 then action = ("Backward")
	elseif _cardIndex == 12 then action = ("Backward")
	elseif _cardIndex == 13 then action = ("TurnRight")
	elseif _cardIndex == 14 then action = ("TurnLeft")
	elseif _cardIndex == 15 then action = ("TurnAround")
	elseif _cardIndex == 16 then action = ("TurnRight")
	elseif _cardIndex == 17 then action = ("TurnLeft")
	elseif _cardIndex == 18 then action = ("TurnAround")
	elseif _cardIndex == 19 then action = ("Backward")
	elseif _cardIndex == 20 then action = ("TurnLeft")
	end
	
	return action
end

AICardPickingSystem.UpdateCards = function(self)
	
	local AIs = self:GetEntities("AI")
	local cards = self:GetEntities("AICard")
	local dealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(dealingSettings[1], "DealingSettings", 0):GetInt2(0)
	
	for i = 1, #AIs do
		
		local cardSetAI = self:GetAIsCardSet(AIs[i], cards)
		
		for card = 1, #cardSetAI do
			
			local cardName = world:GetComponent(cardSetAI[card], "CardAction", 0):GetText(0)
			local playerNumber = world:GetComponent(AIs[i], "PlayerNumber", 0):GetInt(0)
			
			local arrayIndex = (playerNumber - 1) * cardsPerHand + card
			self.CardSet[arrayIndex] = cardName
			--print(cardName)
			--print(cardName, arrayIndex, playerNumber)
		end
	end	
end

AICardPickingSystem.SaveCheckpoints = function(self)
	
	local checkpoints = self:GetEntities("Checkpoint")
	
	for checkpoint = 1, #checkpoints do
		
		local checkpointNumber = world:GetComponent(checkpoints[checkpoint], "Checkpoint", 0):GetInt(0)
		local posX, posY = world:GetComponent(checkpoints[checkpoint], "MapPosition", 0):GetInt2(0)
		local startIndex = (checkpointNumber - 1) * 2 + 1
		
		self.CheckpointPositions[startIndex] = posX
		self.CheckpointPositions[startIndex + 1] = posY
	end
	
	self.NoOfCheckpoints = #checkpoints
end

AICardPickingSystem.SaveMap = function(self)
	
	local mapSizeEntities = self:GetEntities("MapSpecs")
	local mapX, mapY = world:GetComponent(mapSizeEntities[1], "MapSpecs", "SizeX"):GetInt2(0)
	self.MapSizeX = mapX
	self.MapSizeY = mapY
	local tiles = self:GetEntities("TileComp")
	
	for y = 0, mapY - 1 do
		for x = 0, mapX - 1 do
			
			local xIndex = (mapX * y + x) * 2 + 1
			local yIndex = xIndex + 1
			self.RiverDirections[xIndex] = 0
			self.RiverDirections[yIndex] = 0
			
			if world:EntityHasComponent(tiles[mapX * y + x + 1], "Void") then
			
				self.Map[mapX * y + x + 1] = "Void"
				--io.write("O")
				
			elseif world:EntityHasComponent(tiles[mapX * y + x + 1], "NotWalkable") then
				
				self.Map[mapX * y + x + 1] = "NotWalkable"
				--io.write("X")
				
			elseif world:EntityHasComponent(tiles[mapX * y + x + 1], "River") then
				
				self.Map[mapX * y + x + 1] = "River"
				local dirX, dirY, speed = world:GetComponent(tiles[mapX * y + x + 1], "River", 0):GetInt3(0)
				
				
				if dirX == 1 then
					--self.Map[mapX * y + x + 1] = "RiverRight"
					self.RiverDirections[(mapX * y + x) * 2 + 1] = 1
					--io.write("R")
				elseif dirX == -1 then
					--self.Map[mapX * y + x + 1] = "RiverLeft"
					self.RiverDirections[(mapX * y + x) * 2 + 1] = -1
					--io.write("L")
				elseif dirY == 1 then
					--self.Map[mapX * y + x + 1] = "RiverDown"
					self.RiverDirections[(mapX * y + x) * 2 + 2] = 1
					--io.write("D")
				elseif dirY == -1 then
					--self.Map[mapX * y + x + 1] = "RiverUp"
					self.RiverDirections[(mapX * y + x) * 2 + 2] = -1
					--io.write("U")
				else
					print("Error: Could not decide the direction of the river when saving map in AICardPickingSystem")
				end
				
			else
				
				self.Map[mapX * y + x + 1] = ""
				--io.write(" ")
			end
			
			self.TempStones[mapX * y + x + 1] = 0
			self.SimStones[mapX * y + x + 1] = 0
			--if self.TempStones[mapX * y + x + 1] == false then
			--	io.write(" ")
			--else
			--	io.write("x")
			--end
		end
		--io.write("\n")
	end
end

AICardPickingSystem.UpdateTileWalkability = function(self, _entity)
	
	local lifeTime, tileX, tileY = world:GetComponent(_entity, "TileWalkabilityHasChanged", 0):GetInt3(0)
	
	if not self.StoneLifeSpanSet then
		self.StoneLifeSpanSet = true
		self.StoneLifeSpan = lifeTime
	end
	
	self.TempStones[self.MapSizeX * tileY + tileX + 1] = lifeTime
	
	world:KillEntity(_entity)
	
	--io.write("\n\n")
	--for stoneY = 0, self.MapSizeY - 1 do
	--	for stoneX = 0, self.MapSizeX - 1 do
	--		io.write(self.TempStones[self.MapSizeX * stoneY + stoneX + 1])
	--	end
	--	io.write("\n")
	--end
end

AICardPickingSystem.InitPlayerSpecifics = function(self, _aiEntity)

	--local mapSpecs = self:GetEntities("MapSpecs")
	--local noOfSpawnPoints = world:GetComponent(mapSpecs[1], "MapSpecs", "NoOfSpawnpoints"):GetInt(0)
	local playerNo = world:GetComponent(_aiEntity, "PlayerNumber", 0):GetInt(0)
	local unitID = world:GetComponent(_aiEntity, "UnitEntityId", 0):GetInt(0)
	
	local doubleIndex = (playerNo - 1) * 2 + 1
	
	local x, y = world:GetComponent(unitID, "Spawnpoint", 0):GetInt2(0)
	self.Spawnpoints[doubleIndex] 		= x
	self.Spawnpoints[doubleIndex + 1] 	= y
		
	x, y = world:GetComponent(unitID, "MapPosition", 0):GetInt2(0)
	
	-- Check if the AI ended up in a void, if so use its spawnpoint. The AI gets new cards before the units positions are updated.
	local mapX, mapY = self.MapSizeX, self.MapSizeY
	local inVoid = false	
	
	-- If the pos is in inside the boundaries, check if it is in a void.
	if -1 < x and x < mapX and -1 < y and y < mapY then
		if self.Map[mapX * y + x + 1] == "Void" then
			inVoid = true
		end
	-- Else the pos is outside the boundaries, which is also void.
	else
		inVoid = true
	end
	
	
	if inVoid then
		
		--print()
		--print()
		--print()
		--print("Unit ended up in void, using its spawnpoint instead.")
		--print()
		--print()
		--print()
		self.Positions[doubleIndex] 	= self.Spawnpoints[doubleIndex]
		self.Positions[doubleIndex + 1] = self.Spawnpoints[doubleIndex + 1]
	else
		self.Positions[doubleIndex] 	= x
		self.Positions[doubleIndex + 1] = y
	end
	
	x, y = world:GetComponent(unitID, "Direction", 0):GetInt2(0)
	self.Directions[doubleIndex] 		= x
	self.Directions[doubleIndex + 1] 	= y
	
	
	self.TargetCheckpoints[playerNo] = world:GetComponent(unitID, "TargetCheckpoint", 0):GetInt(0)
	
	--print(self.Positions[doubleIndex], self.Positions[doubleIndex + 1], self.Directions[doubleIndex], self.Directions[doubleIndex + 1], self.Spawnpoints[doubleIndex], self.Spawnpoints[doubleIndex + 1], self.TargetCheckpoints[playerNo]))
	
	local red, green, blue = world:GetComponent(unitID, "Color", "X"):GetFloat3()
	
	world:GetComponent(self.SimulationGhosts[playerNo], "Color", "X"):SetFloat3(red, green, blue)
end

AICardPickingSystem.AllocatePlayerArrays = function(self)
	
	local mapSpecs = self:GetEntities("MapSpecs")
	local noOfSpawnPoints = world:GetComponent(mapSpecs[1], "MapSpecs", "NoOfSpawnpoints"):GetInt(0)
	
	for player = 1, noOfSpawnPoints do
		
		self.TargetCheckpoints[player] = 1
		self.SimulationGhosts[player] = world:CreateNewEntity("Ghost")
		world:GetComponent(self.SimulationGhosts[player], "Model", 0):SetModel("cavemenghost", "caveman", 1, false)		
		world:GetComponent(self.SimulationGhosts[player], "Color", "X"):SetFloat3(1, 1, 1)
		
		local doubleIndex = (player - 1) * 2 + 1 
		
		self.Positions[doubleIndex] 		= 0
		self.Positions[doubleIndex + 1] 	= 0
		self.Directions[doubleIndex] 		= 0
		self.Directions[doubleIndex + 1] 	= 0
		self.Spawnpoints[doubleIndex] 		= 0
		self.Spawnpoints[doubleIndex + 1] 	= 0
	end
end

AICardPickingSystem.ResetValuesPreRound = function(self)
	
	local mapSpecs = self:GetEntities("MapSpecs")
	local noOfSpawnPoints = world:GetComponent(mapSpecs[1], "MapSpecs", "NoOfSpawnpoints"):GetInt(0)
	local dealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(dealingSettings[1], "DealingSettings", 0):GetInt2(0)
	
	-- Initialize array holding "BestDist", "BestNextDist" and "CheckpointsReached".
	for player = 1, noOfSpawnPoints do
		
		local trippleIndex = (player - 1) * 3 + 1
		
		self.BestDistances[trippleIndex] = -1
		self.BestDistances[trippleIndex + 1] = -1
		self.BestDistances[trippleIndex + 2] = -1
		
		for card = 1, cardsPerHand do
			self.CardSet[(player - 1) * cardsPerHand + card] = ""
		end
	end
	
	self.PermutationIndex = 1
	self.CurrentAI = 1
	
	self.ChosenCardsArray = {}
	self.ChosenCardsArray.__mode = "k"
end

AICardPickingSystem.EntitiesAdded = function(self, dt, entities)
		
	local gotCards = false
	
	--print("hej ai entity")
	
	for i = 1, #entities do
		
		if world:EntityHasComponent(entities[i], "AICard") then
			
			gotCards = true
			
		-- If an AI was added, do permutations if it is the first AI.
		elseif world:EntityHasComponent(entities[i], "AI") and not self.InitDone then
		
			local dealingSettings = self:GetEntities("DealingSettings")
			local cardsPerHand, cardsToPick = world:GetComponent(dealingSettings[1], "DealingSettings", 0):GetInt2(0)
			
			self.CardsPerHand = cardsPerHand
			self.CardsToPick = cardsToPick
			
			self:SaveCheckpoints()
			self:SaveMap()
			self:AllocatePlayerArrays()
			self.PermutationsArray = CombinationMath.Permutations(cardsPerHand, cardsToPick)
	
--self.SimulationGhost = world:CreateNewEntity("Ghost")
--world:GetComponent(self.SimulationGhost, "Model", 0):SetModel("cavemenghost", "caveman", 1, false)

			self.InitDone = true
			
			--self:InitPlayerSpecifics(entities[i])
			
		elseif world:EntityHasComponent(entities[i], "AutoPickCards") then
			
			print("AIs picking time is over!", (self.PermutationIndex - 1) / self.CardsToPick)
			
			self:SendCards()
			
		elseif world:EntityHasComponent(entities[i], "TileWalkabilityHasChanged") then
			
			self:UpdateTileWalkability(entities[i])
			
		end
		
	end
	
	--print("asldkj")
	
	if gotCards then
	
		self.TimeToPick = true
		self:ResetValuesPreRound()
		self:UpdateCards()
		
		local AIs = self:GetEntities("AI")
		
		for n = 1, #AIs do
			self:InitPlayerSpecifics(AIs[n])
		end
		
		--self:UpdatePlayerComponents()
	end
	--print("entities added done ai")
end

AICardPickingSystem.GetGhostRotation = function(self, dirX, dirZ)

	local	returnRotation	=	0
	if dirX == 1 then
		returnRotation	=	math.pi/2
	end
	if dirX == -1 then
		returnRotation	=	-math.pi/2
	end
	if dirZ == -1 then
		returnRotation	=	-math.pi
	end
	if dirZ == 1 then
		returnRotation	=	0
	end
	
	return returnRotation
end

--AICardPickingSystem.TileHasComponent = function(self, _component, _posX, _posY)
--	
--	local mapSize = self:GetEntities("MapSpecs")
--	local mapSizeComp = world:GetComponent(mapSize[1], "MapSpecs", "SizeX")
--	local mapX, mapY = mapSizeComp:GetInt2()
--	local tiles = self:GetEntities("TileComp")
--	local returnValue
--		
--	if -1 < _posX and _posX < mapX and -1 < _posY and _posY < mapY then
--		returnValue = world:EntityHasComponent(tiles[mapX * _posY + _posX + 1], _component)
--	else
--		if not self.ERROR then
--			print("ERROR, trying to get entity from tile outside the boundaries in AICardPickingSystem.TileHasComponent.")
--			self.ERROR = true
--		end
--		returnValue = false
--	end
--	
--	return returnValue
--end

--AICardPickingSystem.OldSimulateCards = function(self, _playerNumber, _targetCheckpointNumber, _pickedcards)
--	
--	local doubleIndex = (_playerNumber - 1) * 2 + 1 
--	
--	
--	--local posX, posY = world:GetComponent(_unit, "MapPosition", 0):GetInt2(0)
--	--local dirX, dirY = world:GetComponent(_unit, "Direction", 0):GetInt2(0)
--	local posX = self.Positions[doubleIndex]
--	local posY = self.Positions[doubleIndex + 1]
--	local dirX = self.Directions[doubleIndex]
--	local dirY = self.Directions[doubleIndex + 1]
--	
--	--print(posX, posY, dirX, dirY)
--	
--	local fellDown = false	
--	local cardName = ""
--	local checkPointsReached = 0
--	local targetCheckpointNumber = _targetCheckpointNumber
--	
--	for n = 1, #_pickedcards do
--		
--		cardName = _pickedcards[n]
--		
--		if cardName == "Forward" then
--			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 1, false)
--			
--		elseif cardName == "Backward" then
--			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, false, false, 1, false)
--			
--		elseif cardName == "TurnLeft" then
--			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 1)
--			
--		elseif cardName == "TurnRight" then
--			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 3)
--			
--		elseif cardName == "TurnAround" then
--			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 2)
--			
--		elseif cardName == "Guard" or cardName == "SlingShot" then
--			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 0)
--			
--		elseif cardName == "Sprint" then
--			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 2, false)
--			
--		elseif cardName == "Stone" then
--			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 1, false)
--			
--		else
--			print("Warning: Card Not Added In SimulateCards (AI)", cardName)
--		end
--		
--		if fellDown then
--			posX = self.Spawnpoints[doubleIndex]
--			posY = self.Spawnpoints[doubleIndex + 1]
--			break
--		end
--		
--		if self:ReachedCheckpoint(targetCheckpointNumber, posX, posY) then
--			checkPointsReached = checkPointsReached + 1
--			targetCheckpointNumber = targetCheckpointNumber + 1
--		end
--	end
--	
--	return fellDown, checkPointsReached, posX, posY, dirX, dirY
--end

--AICardPickingSystem.SimulateMoveForward = function(self, _posX, _posY, _dirX, _dirY, _forwards, _jump, _iterations, _riverMove)
--	
--	local forward = -1
--	if _forwards then
--		forward = 1
--	end
--	
--	local fellDown = false
--	local posX = _posX
--	local posY = _posY
--	
--	for i = 1, _iterations do
--		
--		posX = posX + _dirX * forward
--		posY = posY + _dirY * forward
--		
--		--if self:LocalMapHasComponent("NotWalkable", posX, posY) then
--		if self:TileHasComponent("NotWalkable", posX, posY) then
--			
--			--print("Stone at", posX, posY)
--			posX = posX - _dirX * forward
--			posY = posY - _dirY * forward
--			
--		--elseif self:LocalMapHasComponent("Void", posX, posY) and not _jump then
--		elseif self:TileHasComponent("Void", posX, posY) and not _jump then
--			
--			fellDown = true
--			if self.PrintSimulation == 1 then
--				print("I will fall down in", posX, posY)
--			end
--			break
--		end
--		
--		--if not _riverMove and i == _iterations and self:LocalMapHasComponent("River", posX, posY) then
--		if not _riverMove and i == _iterations and self:TileHasComponent("River", posX, posY) then
--			
--			local waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(posX, posY)
--			
--			for j = 1, waterSpeed do
--			
--				--if self:LocalMapHasComponent("River", posX + waterDirX, posY + waterDirY) then
--				if self:TileHasComponent("River", posX + waterDirX, posY + waterDirY) then
--					posX = posX + waterDirX
--					posY = posY + waterDirY
--					waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(posX, posY)
--				else
--					fellDown, posX, posY = self:SimulateMoveForward(posX, posY, waterDirX, waterDirY, true, false, 1, true)
--					break
--				end
--				
--				if self.PrintSimulation == 1 then
--					print("I will move in river with X, Y, speed:", waterDirX, waterDirY, waterSpeed)
--				end
--			end
--		end
--	end
--	
--	if not _riverMove and self.PrintSimulation == 1 then
--		print("Pos", posX, posY, "Forw:", _forwards)
--	end
--	
--	return fellDown, posX, posY
--end

--AICardPickingSystem.SimulateTurnLeft = function(self, _posX, _posY, _dirX, _dirY, _iterations)
--	
--	local dirX = _dirX
--	local dirY = _dirY
--	local temp
--	local posX = _posX
--	local posY = _posY
--	local fellDown = false
--	
--	for i = 1, _iterations do
--		
--		temp = dirY
--		dirY = -dirX
--		dirX = temp
--	end
--	
--	if self:TileHasComponent("River", _posX, _posY) then
--		
--		local waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(_posX, _posY)
--		
--		if self.PrintSimulation == 1 then
--			print("I will move in river with X, Y, speed:", waterDirX, waterDirY, waterSpeed)
--		end
--		
--		fellDown, posX, posY = self:SimulateMoveForward(_posX, _posY, waterDirX, waterDirY, true, false, waterSpeed, true)
--		
--		if self.PrintSimulation == 1 then
--			print("Pos", posX, posY, "Dir", dirX, dirY, "with", _iterations)
--		end
--	elseif self.PrintSimulation == 1 then
--		print("Dir", dirX, dirY, "with", _iterations, "turn left iterations")
--	end
--	
--	return fellDown, posX, posY, dirX, dirY
--end

--AICardPickingSystem.AddExtraCostIfFellDown = function(self, _fellDown, _cardsLeftToPick)
--	
--	if _fellDown then
--		local extraCost = _cardsLeftToPick + 1
--		return extraCost
--	else
--		return 0
--	end
--end

--AICardPickingSystem.LocalMapHasComponent = function(self, _component, _posX, _posY)
--	
--	--local mapSize = self:GetEntities("MapSpecs")
--	--local mapSizeComp = world:GetComponent(mapSize[1], "MapSpecs", "SizeX")
--	--local mapX, mapY = mapSizeComp:GetInt2()
--	--local tiles = self:GetEntities("TileComp")
--	local mapX, mapY = self.MapSizeX, self.MapSizeY
--	local returnValue
--		
--	if -1 < _posX and _posX < mapX and -1 < _posY and _posY < mapY then
--		--print(self.Map[mapX * _posY + _posX + 1], _component)
--		if self.Map[mapX * _posY + _posX + 1] == _component then
--			returnValue = true
--			--print("true")
--		end
--		--print("false")
--	else
--		if not self.ERROR then
--			print("ERROR, trying to get entity from tile outside the boundaries in AICardPickingSystem.TileHasComponent.")
--			self.ERROR = true
--		end
--		returnValue = false
--	end
--	
--	return returnValue
--end

--AICardPickingSystem.AIPickCards = function( self, _cardSetAI, _unitID )
--	
--	local pickedCards = {}
--	pickedCards.__mode = "k"
--	
--	local dealingSettings = self:GetEntities("DealingSettings")
--	local cardsPerHand, cardsToPick = world:GetComponent(dealingSettings[1], "DealingSettings", 0):GetInt2(0)
--		
--	if self.AICheat then
--		self:ChangeTheCards(cardsPerHand, _cardSetAI)
--	end
--	
--	
--	--local startTime, endTime, timetaken, timetaken2		
--	--local lengthArray = string.len(self.PermutationsArray)
--	--local jump = self.CardsToSimulate
--	--local cardsToSimMinusOne = self.CardsToSimulate - 1		
--	--
--	--startTime = os.clock()
--	--local charArray = self.PermutationsArray
--	---- This is faster than using the global value.
--	--
--	--for permutationIndex = 1, lengthArray, jump do
--	--	for cardIndex = 0, cardsToSimMinusOne do
--	--		local charVar = string.byte(charArray, permutationIndex + cardIndex)
--	--	end
--	--end
--	--
--	--endTime = os.clock()
--	--timetaken = (endTime - startTime)
--	--
--	--print("Permutations took", timetaken, "microseconds")
--	--print("Permutations took", timetaken2, "microseconds")
--	
--	
--	
--	
--	
--	--local forwards = self:GetAllCardsOf(_cardSetAI, "Forward")
--	--local backwards = self:GetAllCardsOf(_cardSetAI, "Backward")
--	--local turnLefts = self:GetAllCardsOf(_cardSetAI, "TurnLeft")
--	--local turnRights = self:GetAllCardsOf(_cardSetAI, "TurnRight")
--	--local turnArounds = self:GetAllCardsOf(_cardSetAI, "TurnAround")
--	--local sprints = self:GetAllCardsOf(_cardSetAI, "Sprint")
--	--local shots = self:GetAllCardsOf(_cardSetAI, "SlingShot")
--	
--	local simFellDown, simPosX, simPosY, simDirX, simDirY
--	local cardsToSim = {}
--	cardsToSim.__mode = "k"
--	local bestCardIds = {}
--	bestCardIds.__mode = "k"
--	local bestCardId, bestDist, bestNextDist
--	local dist, nextDist
--	
--	local posX, posY = world:GetComponent(_unitID, "MapPosition", 0):GetInt2()
--	local dirX, dirY = world:GetComponent(_unitID, "Direction", 0):GetInt2()
--	local fellDown = false
--	
--	local noOfCardsToSim = self.CardsToSimulate
--	
--	for i = 1, cardsToPick do
--		
--		-- Get target. Needs to be done after every card as we may have reached a checkpoint.
--		local targetX, targetY = self:GetTargetPosition(self:GetEntities("Checkpoint"), world:GetComponent(_unitID, "TargetCheckpoint", 0):GetInt())
--		local cardsPicked = #pickedCards
--		local cardsLeftToPick = cardsToPick - cardsPicked
--		
--		-- TODO: Add method to vaska cards if we will fall down.
--		--if fellDown then
--		--	Vaska kort!
--		--end
--		
--		local cardsLeftInHand = cardsPerHand - cardsPicked
--		local noToGet = math.min(cardsLeftToPick, noOfCardsToSim)
--		
--		local charArray = ''
--		
--		charArray = CombinationMath.Permutations(cardsLeftInHand, noToGet)
--		
--		-- Init variables to speed up usage of permutations.
--		local lengthArray = string.len(charArray)
--		local cardsToSimMinusOne = noToGet - 1
--		
--		--for n = 1, noToGet do
--		--	cardsToSim[n] = _cardSetAI[n]
--		--	bestCardIds[n] = n
--		--end
--		dist, simFellDown, simPosX, simPosY, simDirX, simDirY = self:GetSimDist(_unitID, posX, posY, dirX, dirY, targetX, targetY, cardsToSim, cardsLeftToPick)
--		
--		nextDist = self:GetSimNextDist(simFellDown, simPosX, simPosY, simDirX, simDirY, targetX, targetY, cardsLeftToPick)
--		
--		--for n = 1, #cardsToSim do
--		--	cardName = world:GetComponent(cardsToSim[n], "CardAction", 0):GetText()
--		--	print(cardName)
--		--end
--		--print("Set first cards")
--		for n = 1, noToGet do
--			table.remove(cardsToSim, 1)
--		end
--		bestNextDist = nextDist;
--		bestDist = dist;
--		
--		bestCardId = 1
--		
--		local firstCardId
--		
--		for permutationIndex = 1, lengthArray, noToGet do
--			
--			--local cardIds = {}
--			--cardIds.__mode = "k"
--			
--			for cardIndex = 0, cardsToSimMinusOne do
--				local charVar = string.byte(charArray, permutationIndex + cardIndex)
--				cardsToSim[cardIndex + 1] = _cardSetAI[charVar]
--				--cardIds[cardIndex + 1] = charVar
--				
--				if cardIndex == 0 then
--					firstCardId = charVar
--				end
--			end
--			
--			dist, simFellDown, simPosX, simPosY, simDirX, simDirY = self:GetSimDist(_unitID, posX, posY, dirX, dirY, targetX, targetY, cardsToSim, cardsLeftToPick)
--			
--			if dist <= bestDist then
--				
--				-- TODO: Only do nextDist(forward) if have forward cards, only do nextDist(backward) if have backward cards.
--				-- TODO: Don't use walk-cards if we end up at the same spot.
--				
--				nextDist = self:GetSimNextDist(simFellDown, simPosX, simPosY, simDirX, simDirY, targetX, targetY, cardsLeftToPick)
--									
--				-- If the distance is the same as the best and the "nextDist" is better, then a better card has been found.
--				if dist == bestDist and nextDist < bestNextDist then
--					
--					bestCardId = firstCardId
--					bestNextDist = nextDist
--					
--					--for bi = 1, #cardIds do
--					--	bestCardIds[bi] = cardIds[bi]
--					--end
--					
--					--for n = 1, #cardsToSim do
--					--	cardName = world:GetComponent(cardsToSim[n], "CardAction", 0):GetText()
--					--	print(cardName)
--					--end
--					--
--					--print("New best found")
--				
--				-- TODO: Why is this in an elseif, some further things to be added???
--				-- Else if the dist is better, the card is better.
--				elseif dist < bestDist then
--					
--					bestCardId = firstCardId
--					bestNextDist = nextDist
--					bestDist = dist
--					
--					--for bi = 1, #cardIds do
--					--	bestCardIds[bi] = cardIds[bi]
--					--end
--					
--					--for n = 1, #cardsToSim do
--					--	cardName = world:GetComponent(cardsToSim[n], "CardAction", 0):GetText()
--					--	print(cardName)
--					--end
--					--
--					--print("New best found")
--				end
--			end
--			
--			for cardIndex = 0, cardsToSimMinusOne do
--				table.remove(cardsToSim, 1)
--			end
--		end
--		
--		--if (cardsLeftToPick == noOfCardsToSim) then
--		--	
--		--	for n = 1, #bestCardIds do
--		--		pickedCards[#pickedCards + 1] = _cardSetAI[bestCardIds[n]]
--		--		--print("Rest of cards picked", bestCardIds[n])
--		--	end
--		--	
--		--	break
--		--else
--			-- Choose card, and simulate the choosen card to use the positions to choose the next card.
--			pickedCards[#pickedCards + 1] = _cardSetAI[bestCardId]
--			cardsToSim[#cardsToSim + 1] = _cardSetAI[bestCardId]
--			fellDown, posX, posY, dirX, dirY = self:SimulateCardsFromPos(_unitID, posX, posY, dirX, dirY, cardsToSim)
--			table.remove(cardsToSim, 1)
--			table.remove(_cardSetAI, bestCardId)
--			--print("card picked", bestCardId, bestCardIds[1])
--		--end
--	end		
--	return pickedCards
--end

--AICardPickingSystem.SendCards = function(self, _player, _pickedcards)
--	
--	--print("AI Send Cards")
--	local unit = world:GetComponent(_player, "UnitEntityId", "Id"):GetInt()
--	
--	world:CreateComponentAndAddTo("HasSelectedCards", _player)
--	world:CreateComponentAndAddTo("UnitSelectedCards", unit)
--	
--	local DealingSettings = self:GetEntities("DealingSettings")
--	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
--
--	for i = 1, cardsToPick do
--		
--		local action = world:GetComponent(_pickedcards[i], "CardAction", 0):GetText()
--		local prio = world:GetComponent(_pickedcards[i], "CardPrio", 0):GetInt()
--		--print("AI Action: " .. action .. " - Prio: " .. prio)
--	
--		world:RemoveComponentFrom("DealtCard", _pickedcards[i])
--		world:RemoveComponentFrom("AICard", _pickedcards[i])
--		world:CreateComponentAndAddTo("CardStep", _pickedcards[i])
--		world:SetComponent(_pickedcards[i], "CardStep", "Step", i)
--		world:SetComponent(_pickedcards[i], "CardStep", "UnitEntityId", unit)
--	end
--
--	local id = world:CreateNewEntity()
--	world:CreateComponentAndAddTo("NotifyStartNewRound", id)
--	world:GetComponent(id, "NotifyStartNewRound", "IsAI"):SetBool(true) 
--end