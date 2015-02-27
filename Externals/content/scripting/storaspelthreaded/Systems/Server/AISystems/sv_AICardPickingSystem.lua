AICardPickingSystem = System()
AICardPickingSystem.PrintSimulation = 0
AICardPickingSystem.AICheat = 0
AICardPickingSystem.PermutationsDone = false
AICardPickingSystem.PermutationIndex = 1
AICardPickingSystem.PermutationsArray = ''
AICardPickingSystem.ChosenCardsArray = {}
AICardPickingSystem.ChosenCardsArray.__mode = "k"
AICardPickingSystem.BestDistances = {}
AICardPickingSystem.BestDistances.__mode = "k"
AICardPickingSystem.TimeLimitPerUpdateInMs = 1.0

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
end

AICardPickingSystem.Update = function(self, dt)
	
	local AIs = self:GetEntities("AI")
	
	if #AIs == 0 or #self.PermutationsArray <= self.PermutationIndex then
		return
	end
	
	local cards = self:GetEntities("AICard")
	local dealingSettings = self:GetEntities("DealingSettings")	
	local cardsPerHand, cardsToPick = world:GetComponent(dealingSettings[1], "DealingSettings", 0):GetInt2(0)
	
	if #cards >= #AIs * cardsPerHand then
		
		local startTime, endTime
		local timeTaken = 0.0
		local timeLimit = self.TimeLimitPerUpdateInMs
		
		local dealingSettings = self:GetEntities("DealingSettings")
		
		startTime = os.clock()
		while timeTaken < timeLimit do
			
			for i = 1, #AIs do
				
				local unitID = world:GetComponent(AIs[i], "UnitEntityId", 0):GetInt()
				local playerNumber = world:GetComponent(AIs[i], "PlayerNumber", 0):GetInt()
				local cardSetAI = self:GetAIsCardSet(AIs[i], cards)
				self:PickCards(unitID, playerNumber, cardSetAI)
			end
			
			self.PermutationIndex = self.PermutationIndex + cardsToPick
			
			endTime = os.clock()
			timeTaken = (endTime - startTime) * 1000
		end
		
		--io.write("Picking cards took ", timeTaken, " milliseconds. ", (self.PermutationIndex - 1) / cardsToPick, " loops.\n")
		
		--for i = 1, #AIs do
		--	
		--	--local cpTargetNr = world:GetComponent(unitID, "TargetCheckpoint", 0):GetInt()
		--	
		--	--Fetch the cards which is relevant to the current AI
		--	local cardSetAI = self:GetAIsCardSet(AIs[i], cards)
		--	
		--	if #cardSetAI == cardsPerHand then		
		--	
		--		PotentialFieldHandler.UseMyPF(i)
		--		
		--		local unitID = world:GetComponent(AIs[i], "UnitEntityId", 0):GetInt()
		--		
		--		--local startTime, endTime, timeTaken
		--		
		--		--startTime = os.clock()
		--		
		--		--local pickedCards = self:AIPickCards(cardSetAI, unitID)
		--		
		--		
		--		--endTime = os.clock()
		--		--timetaken = (endTime - startTime) * 1000
		--		
		--		--print("Picking cards took", timetaken, "milliseconds")
		--		
		--		
		--		
		--		
		--		
		--		--if #pickedCards == cardsToPick then	
		--		--	
		--		--	self:SendCards(AIs[i], PickedCards)
		--		--else
		--		--	print("ERROR: AI HAVE NOT SELECTED CORRECT NUMBER OF CARDS", #pickedCards)
		--		--end
		--	else
		--		print("Error: AI has not received correct number of cards. It has", #cardSetAI, "cards")
		--	end
		--end
		--
		--print("AI Done")
	end
end

--AICardPickingSystem.GetTargetPosition = function(self, _unit)
--	
--	local checkpointTiles = self:GetEntities("Checkpoint")
--	local cpTargetNr = world:GetComponent(_unitID, "TargetCheckpoint", 0):GetInt()
--	local targetPositionX, targetPositionY
--	local asd = false
--	for k = 1, #checkpointTiles do
--		local target = world:GetComponent(checkpointTiles[k], "Checkpoint", 0):GetInt()
--		
--		if target == cpTargetNr then
--			targetPositionX, targetPositionY = world:GetComponent(checkpointTiles[k], "MapPosition", 0):GetInt2()
--			asd = true
--		end
--	end
--	if asd == false then
--		targetPositionX = 0
--		targetPositionY = 0
--		print("Error: Could not find target checkpoint int AICardPickingSystem")
--	end
--	
--	return targetPositionX, targetPositionY
--end

AICardPickingSystem.GetCheckpointPos = function(self, _targetCheckpointNumber)
	
	local checkpointTiles = self:GetEntities("Checkpoint")
	
	for k = 1, #checkpointTiles do
		local checkpointNumber = world:GetComponent(checkpointTiles[k], "Checkpoint", 0):GetInt(0)
		
		if checkpointNumber == _targetCheckpointNumber then
			return world:GetComponent(checkpointTiles[k], "MapPosition", 0):GetInt2(0)
		end
	end
	
	-- Next checkpoint was not found, that means that the AI will probably get to the finish, return the same target so it will stay close to the finish if it is pushed away.
	return self:GetCheckpointPos(_targetCheckpointNumber - 1)
end

AICardPickingSystem.ReachedCheckpoint = function(self, _targetCheckpointNumber, _posX, _posY)
	
	local checkpoints = self:GetEntities("Checkpoint")
	
	for checkpoint = 1, #checkpoints do
		
		local checkpointNumber = world:GetComponent(checkpoints[checkpoint], "Checkpoint", 0):GetInt(0)
		
		if checkpointNumber == _targetCheckpointNumber then
			
			local targetX, targetY = world:GetComponent(checkpoints[checkpoint], "MapPosition", 0):GetInt2(0)
			
			if _posX == targetX and _posY == targetY then
				return true
			end
		end
	end
	
	return false
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

AICardPickingSystem.PickCards = function( self, _unitID, _playerNumber, _cardSetAI )
	
	local dealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(dealingSettings[1], "DealingSettings", 0):GetInt2(0)
	
	local cardsToSim = {}
	cardsToSim.__mode = "k"
	
	local targetNr = world:GetComponent(_unitID, "TargetCheckpoint", 0):GetInt(0)
	local targetX, targetY = self:GetCheckpointPos(targetNr)
	
	if self.AICheat == 1 then
		self:ChangeTheCards(cardsPerHand, _cardSetAI)
	end
	
	-- Get the cards to simulate by accessing the permutations array.
	for cardIndex = 0, cardsToPick - 1 do	
		
		local charVar = string.byte(self.PermutationsArray, self.PermutationIndex + cardIndex)
		cardsToSim[cardIndex + 1] = _cardSetAI[charVar]
		--local cardaction = world:GetComponent(cardsToSim[cardIndex + 1], "CardAction", 0):GetText()
		--print(cardaction)
		
	end
	
	-- Get the distance from where we end up using the cards.
	local simFellDown, simCPsReached, simPosX, simPosY, simDirX, simDirY = self:SimulateCards(_unitID, targetNr, cardsToSim)
	
	-- If a checkpoint has been reached, change the target.
	if 0 < simCPsReached then
		
		--print("checkpoint reached in the middle of a round")
		--io.write("target before: ", targetX, ", ", targetY, ". SimCPsReached: ", simCPsReached, "\n")
		targetNr = targetNr + simCPsReached
		targetX, targetY = self:GetCheckpointPos(targetNr)
		--io.write("target after: ", targetX, ", ", targetY, "\n")
	end
	
	local dist = PathfinderHandler.GeneratePath(simPosX, simPosY, targetX, targetY)
	
	-- TODO: Add method to vaska cards if we will fall down.
	--if fellDown then
	--	Vaska kort!
	--end
	-- TODO: Only do nextDist(forward) if have forward cards, only do nextDist(backward) if have backward cards.
	-- TODO: Don't use walk-cards if we end up at the same spot.
		
	local bestDist = self.BestDistances[_playerNumber * 3 - 2]
	local bestCPsReached = self.BestDistances[_playerNumber * 3]
	
	local newRound = false
	if self.PermutationIndex == 1 then
		newRound = true
	end
	
	-- If this is the first update on a new round, or if more checkpoints was reached, or if the distance is better than the previously best solution found, then get the nextDist.
	if newRound 
	or bestCPsReached < simCPsReached 
	or (bestCPsReached == simCPsReached and dist <= bestDist) then
		
		-- Get the distance from a cell one tile ahead, then get distance from one tile behind and compare the two. Save the best.
		local nextDist = PathfinderHandler.GeneratePath(simPosX + simDirX, simPosY + simDirY, targetX, targetY)
		nextDist = math.min(nextDist, PathfinderHandler.GeneratePath(simPosX - simDirX, simPosY - simDirY, targetX, targetY))
		
		local bestNextDist = self.BestDistances[_playerNumber * 3 - 1]
		
		-- Update the best solution only if the dist is better or if nextDist is better than nextDist (and dist is equal to bestDist).
		if newRound 
		or bestCPsReached < simCPsReached
		or dist < bestDist 
		or nextDist < bestNextDist then
			
			self.BestDistances[_playerNumber * 3 - 2] = dist
			self.BestDistances[_playerNumber * 3 - 1] = nextDist
			self.BestDistances[_playerNumber * 3] = simCPsReached
			
			--io.write("\n\n")
			-- Save the cards which where used.
			for card = 1, cardsToPick do
				
				local charVar = string.byte(self.PermutationsArray, self.PermutationIndex + card - 1)
				local arrayIndex = (_playerNumber - 1) * cardsToPick + card
				
				self.ChosenCardsArray[arrayIndex] = charVar
				--io.write(self.ChosenCardsArray[arrayIndex], ", ")
			end
			--io.write("\n SimCPsReached: ", simCPsReached, ", dist: ", dist, ", nextDist: ", nextDist, "\n")
		end
	end
end

AICardPickingSystem.AIPickCards = function( self, _cardSetAI, _unitID )
	
	local pickedCards = {}
	pickedCards.__mode = "k"
	
	local dealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(dealingSettings[1], "DealingSettings", 0):GetInt2(0)
		
	if self.AICheat == 1 then
		self:ChangeTheCards(cardsPerHand, _cardSetAI)
	end
	
	
	--local startTime, endTime, timetaken, timetaken2		
	--local lengthArray = string.len(self.PermutationsArray)
	--local jump = self.CardsToSimulate
	--local cardsToSimMinusOne = self.CardsToSimulate - 1		
	--
	--startTime = os.clock()
	--local charArray = self.PermutationsArray
	---- This is faster than using the global value.
	--
	--for permutationIndex = 1, lengthArray, jump do
	--	for cardIndex = 0, cardsToSimMinusOne do
	--		local charVar = string.byte(charArray, permutationIndex + cardIndex)
	--	end
	--end
	--
	--endTime = os.clock()
	--timetaken = (endTime - startTime) * 1000000
	--
	--print("Permutations took", timetaken, "microseconds")
	--print("Permutations took", timetaken2, "microseconds")
	
	
	
	
	
	--local forwards = self:GetAllCardsOf(_cardSetAI, "Forward")
	--local backwards = self:GetAllCardsOf(_cardSetAI, "Backward")
	--local turnLefts = self:GetAllCardsOf(_cardSetAI, "TurnLeft")
	--local turnRights = self:GetAllCardsOf(_cardSetAI, "TurnRight")
	--local turnArounds = self:GetAllCardsOf(_cardSetAI, "TurnAround")
	--local sprints = self:GetAllCardsOf(_cardSetAI, "Sprint")
	--local shots = self:GetAllCardsOf(_cardSetAI, "SlingShot")
	
	local simFellDown, simPosX, simPosY, simDirX, simDirY
	local cardsToSim = {}
	cardsToSim.__mode = "k"
	local bestCardIds = {}
	bestCardIds.__mode = "k"
	local bestCardId, bestDist, bestNextDist
	local dist, nextDist
	
	local posX, posY = world:GetComponent(_unitID, "MapPosition", 0):GetInt2()
	local dirX, dirY = world:GetComponent(_unitID, "Direction", 0):GetInt2()
	local fellDown = false
	
	local noOfCardsToSim = self.CardsToSimulate
	
	for i = 1, cardsToPick do
		
		-- Get target. Needs to be done after every card as we may have reached a checkpoint.
		local targetX, targetY = self:GetTargetPosition(self:GetEntities("Checkpoint"), world:GetComponent(_unitID, "TargetCheckpoint", 0):GetInt())
		local cardsPicked = #pickedCards
		local cardsLeftToPick = cardsToPick - cardsPicked
		
		-- TODO: Add method to vaska cards if we will fall down.
		--if fellDown then
		--	Vaska kort!
		--end
		
		local cardsLeftInHand = cardsPerHand - cardsPicked
		local noToGet = math.min(cardsLeftToPick, noOfCardsToSim)
		
		local charArray = ''
		
		charArray = CombinationMath.Permutations(cardsLeftInHand, noToGet)
		
		-- Init variables to speed up usage of permutations.
		local lengthArray = string.len(charArray)
		local cardsToSimMinusOne = noToGet - 1
		
		--for n = 1, noToGet do
		--	cardsToSim[n] = _cardSetAI[n]
		--	bestCardIds[n] = n
		--end
		dist, simFellDown, simPosX, simPosY, simDirX, simDirY = self:GetSimDist(_unitID, posX, posY, dirX, dirY, targetX, targetY, cardsToSim, cardsLeftToPick)
		
		nextDist = self:GetSimNextDist(simFellDown, simPosX, simPosY, simDirX, simDirY, targetX, targetY, cardsLeftToPick)
		
		--for n = 1, #cardsToSim do
		--	cardName = world:GetComponent(cardsToSim[n], "CardAction", 0):GetText()
		--	print(cardName)
		--end
		--print("Set first cards")
		for n = 1, noToGet do
			table.remove(cardsToSim, 1)
		end
		bestNextDist = nextDist;
		bestDist = dist;
		
		bestCardId = 1
		
		local firstCardId
		
		for permutationIndex = 1, lengthArray, noToGet do
			
			--local cardIds = {}
			--cardIds.__mode = "k"
			
			for cardIndex = 0, cardsToSimMinusOne do
				local charVar = string.byte(charArray, permutationIndex + cardIndex)
				cardsToSim[cardIndex + 1] = _cardSetAI[charVar]
				--cardIds[cardIndex + 1] = charVar
				
				if cardIndex == 0 then
					firstCardId = charVar
				end
			end
			
			dist, simFellDown, simPosX, simPosY, simDirX, simDirY = self:GetSimDist(_unitID, posX, posY, dirX, dirY, targetX, targetY, cardsToSim, cardsLeftToPick)
			
			if dist <= bestDist then
				
				-- TODO: Only do nextDist(forward) if have forward cards, only do nextDist(backward) if have backward cards.
				-- TODO: Don't use walk-cards if we end up at the same spot.
				
				nextDist = self:GetSimNextDist(simFellDown, simPosX, simPosY, simDirX, simDirY, targetX, targetY, cardsLeftToPick)
									
				-- If the distance is the same as the best and the "nextDist" is better, then a better card has been found.
				if dist == bestDist and nextDist < bestNextDist then
					
					bestCardId = firstCardId
					bestNextDist = nextDist
					
					--for bi = 1, #cardIds do
					--	bestCardIds[bi] = cardIds[bi]
					--end
					
					--for n = 1, #cardsToSim do
					--	cardName = world:GetComponent(cardsToSim[n], "CardAction", 0):GetText()
					--	print(cardName)
					--end
					--
					--print("New best found")
				
				-- TODO: Why is this in an elseif, some further things to be added???
				-- Else if the dist is better, the card is better.
				elseif dist < bestDist then
					
					bestCardId = firstCardId
					bestNextDist = nextDist
					bestDist = dist
					
					--for bi = 1, #cardIds do
					--	bestCardIds[bi] = cardIds[bi]
					--end
					
					--for n = 1, #cardsToSim do
					--	cardName = world:GetComponent(cardsToSim[n], "CardAction", 0):GetText()
					--	print(cardName)
					--end
					--
					--print("New best found")
				end
			end
			
			for cardIndex = 0, cardsToSimMinusOne do
				table.remove(cardsToSim, 1)
			end
		end
		
		--if (cardsLeftToPick == noOfCardsToSim) then
		--	
		--	for n = 1, #bestCardIds do
		--		pickedCards[#pickedCards + 1] = _cardSetAI[bestCardIds[n]]
		--		--print("Rest of cards picked", bestCardIds[n])
		--	end
		--	
		--	break
		--else
			-- Choose card, and simulate the choosen card to use the positions to choose the next card.
			pickedCards[#pickedCards + 1] = _cardSetAI[bestCardId]
			cardsToSim[#cardsToSim + 1] = _cardSetAI[bestCardId]
			fellDown, posX, posY, dirX, dirY = self:SimulateCardsFromPos(_unitID, posX, posY, dirX, dirY, cardsToSim)
			table.remove(cardsToSim, 1)
			table.remove(_cardSetAI, bestCardId)
			--print("card picked", bestCardId, bestCardIds[1])
		--end
	end		
	return pickedCards
end

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

AICardPickingSystem.SendCards = function(self, _player)
	
	--print("AI Send Cards")
	local unit = world:GetComponent(_player, "UnitEntityId", "Id"):GetInt()
	
	world:CreateComponentAndAddTo("HasSelectedCards", _player)
	world:CreateComponentAndAddTo("UnitSelectedCards", unit)
	
	local DealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
	
	local cardSet = self:GetAIsCardSet(_player, self:GetEntities("AICard"))
	local playerNumber = world:GetComponent(_player, "PlayerNumber", 0):GetInt(0)

	for card = 1, cardsToPick do
		
		local arrayIndex = (playerNumber - 1) * cardsToPick + card
		local cardIndex = self.ChosenCardsArray[arrayIndex]
		
		local action = world:GetComponent(cardSet[cardIndex], "CardAction", 0):GetText()
		local prio = world:GetComponent(cardSet[cardIndex], "CardPrio", 0):GetInt()
		--print("AI Action: " .. action .. " - Prio: " .. prio)
		
		world:RemoveComponentFrom("DealtCard", cardSet[cardIndex])
		world:RemoveComponentFrom("AICard", cardSet[cardIndex])
		world:CreateComponentAndAddTo("CardStep", cardSet[cardIndex])
		world:SetComponent(cardSet[cardIndex], "CardStep", "Step", card)
		world:SetComponent(cardSet[cardIndex], "CardStep", "UnitEntityId", unit)
	end

	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("NotifyStartNewRound", id)
	world:GetComponent(id, "NotifyStartNewRound", "IsAI"):SetBool(true) 
end

AICardPickingSystem.SimulateCards = function(self, _unit, _targetCheckpointNumber, _pickedcards)
	
	--local posX, posY, dirX, dirY = _posX, _posY, _dirX, _dirY
	
	local posX, posY = world:GetComponent(_unit, "MapPosition", 0):GetInt2(0)
	local dirX, dirY = world:GetComponent(_unit, "Direction", 0):GetInt2(0)
	
	local fellDown = false
	
	if self.PrintSimulation == 1 then
		print()
		print("----------------- NEW SIMULATION STARTED --------------------")
	end
	
	local cardName = ""
	
	local checkPointsReached = 0
	local targetCheckpointNumber = _targetCheckpointNumber
	
	for n = 1, #_pickedcards do
		
		if type(_pickedcards[n]) == "string" then
			cardName = _pickedcards[n]
		else
			cardName = world:GetComponent(_pickedcards[n], "CardAction", 0):GetText()
		end
		
		if self.PrintSimulation == 1 then
			print()
			print("Card number:", n, "is", cardName)
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
			
		elseif cardName == "Guard" or cardName == "SlingShot" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 0)
			
		elseif cardName == "Sprint" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 2, false)
			
		elseif cardName == "Stone" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 1, false)
			
		--elseif cardName == "Leap" then
		--	
		--	fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, true, 2, false)
			
		else
		
			--print("ERROR: CARD NOT ADDED IN SIMULATE CARDS", cardName)
		end
		
		if fellDown then
			
			posX, posY = world:GetComponent(_unit, "Spawnpoint", 0):GetInt2()
			break
		end
		
		if self:ReachedCheckpoint(targetCheckpointNumber, posX, posY) then
			
			checkPointsReached = checkPointsReached + 1
			targetCheckpointNumber = targetCheckpointNumber + 1
		end
	end
	
	if self.PrintSimulation == 1 then
		print("I will end up at:", posX, posY, "with dir:", dirX, dirY)
	end
	
	return fellDown, checkPointsReached, posX, posY, dirX, dirY
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
			
			--print("Stone at", posX, posY)
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
		print("ERROR, trying to get entity from tile outside the boundaries in AICardPickingSystem.TileHasComponent.")
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

AICardPickingSystem.AddExtraCostIfFellDown = function(self, _fellDown, _cardsLeftToPick)
	
	if _fellDown then
		local extraCost = _cardsLeftToPick + 1
		return extraCost
	else
		return 0
	end
end

AICardPickingSystem.ChangeTheCards = function(self, _cardsPerHand, _cardSet)
	for j = 1, _cardsPerHand do
		
		local cardactioncomp = world:GetComponent(_cardSet[j], "CardAction", 0)
		
			if j == 1 then cardactioncomp:SetText("Forward")
		elseif j == 2 then cardactioncomp:SetText("Forward")
		elseif j == 3 then cardactioncomp:SetText("Backward")
		elseif j == 4 then cardactioncomp:SetText("Backward")
		elseif j == 5 then cardactioncomp:SetText("TurnRight")
		elseif j == 6 then cardactioncomp:SetText("TurnLeft")
		elseif j == 7 then cardactioncomp:SetText("TurnAround")
		elseif j == 8 then cardactioncomp:SetText("Forward")
		elseif j == 9 then cardactioncomp:SetText("Forward")
		elseif j == 10 then cardactioncomp:SetText("Forward")
		elseif j == 11 then cardactioncomp:SetText("Backward")
		elseif j == 12 then cardactioncomp:SetText("Backward")
		elseif j == 13 then cardactioncomp:SetText("TurnRight")
		elseif j == 14 then cardactioncomp:SetText("TurnLeft")
		elseif j == 15 then cardactioncomp:SetText("TurnAround")
		elseif j == 16 then cardactioncomp:SetText("TurnRight")
		elseif j == 17 then cardactioncomp:SetText("TurnLeft")
		elseif j == 18 then cardactioncomp:SetText("TurnAround")
		elseif j == 19 then cardactioncomp:SetText("Backward")
		elseif j == 20 then cardactioncomp:SetText("TurnLeft")
		end
	end
end

---- Simulate the playing of the nth card and return the pathfinder's value from that position to the target.
--AICardPickingSystem.GetSimDist = function(self, _unitID, _posX, _posY, _dirX, _dirY, _targetX, _targetY, _cardsToSim)
--
--	local simFellDown, simPosX, simPosY, simDirX, simDirY = self:SimulateCardsFromPos(_unitID, _posX, _posY, _dirX, _dirY, _cardsToSim)
--	local dist = PathfinderHandler.GeneratePath(simPosX, simPosY, _targetX, _targetY)
--	
--	---- Add extraCost if the AI will fall down by playing this card.
--	--dist = dist + self:AddExtraCostIfFellDown(simFellDown, _cardsLeftToPick)
--	
--	return dist, simFellDown, simPosX, simPosY, simDirX, simDirY
--end
--
--AICardPickingSystem.GetSimNextDist = function(self, _simFellDown, _simPosX, _simPosY, _simDirX, _simDirY, _targetX, _targetY)
--	
--	-- Get the distance from a cell as if we have walked forward, then get distance as if we walked forward and compare the two. Save the best.
--	local nextDist = PathfinderHandler.GeneratePath(_simPosX + _simDirX, _simPosY + _simDirY, _targetX, _targetY)
--	nextDist = math.min(nextDist, PathfinderHandler.GeneratePath(_simPosX - _simDirX, _simPosY - _simDirY, _targetX, _targetY))
--	
--	---- Add extraCost if the AI fell down by playing the card.
--	--nextDist = nextDist + self:AddExtraCostIfFellDown(_simFellDown, _cardsLeftToPick)
--	
--	return nextDist
--end

AICardPickingSystem.ResetValues = function(self)
	
	local mapSpecs = self:GetEntities("MapSpecs")
	local noOfSpawnPoints = world:GetComponent(mapSpecs[1], "MapSpecs", "NoOfSpawnpoints"):GetInt(0)
	
	-- Initialize array holding "BestDist", "BestNextDist" and "CheckpointsReached".
	for player = 1, noOfSpawnPoints do
		self.BestDistances[player * 3 - 2] = -1
		self.BestDistances[player * 3 - 1] = -1
		self.BestDistances[player * 3] = -1
	end
	
	self.PermutationIndex = 1
	
	self.ChosenCardsArray = {}
	self.ChosenCardsArray.__mode = "k"
end

AICardPickingSystem.EntitiesAdded = function(self, dt, entities)

	for i = 1, #entities do
		
		if world:EntityHasComponent(entities[i], "DealtCard") then
			local playerid = world:GetComponent(entities[i], "DealtCard", 0)
			local id = playerid:GetInt()
			local plynum = world:GetComponent(id, "PlayerNumber", 0):GetInt()
			local card = world:GetComponent(entities[i], "CardAction", 0):GetText()
		-- If an AI was added, do permutations if it is the first AI.
		elseif world:EntityHasComponent(entities[i], "AI") and not self.PermutationsDone then
		
			local dealingSettings = self:GetEntities("DealingSettings")
			local cardsPerHand, cardsToPick = world:GetComponent(dealingSettings[1], "DealingSettings", 0):GetInt2(0)
			
			if not self.PermutationsDone then
				
				self:ResetValues()
				self.PermutationsArray = CombinationMath.Permutations(cardsPerHand, cardsToPick)
				self.PermutationsDone = true
			end
			
		elseif world:EntityHasComponent(entities[i], "AutoPickCards") then
			
			local AIs = self:GetEntities("AI")
			print("AIs picking time is over!", (self.PermutationIndex - 1) / 5)
			
			for n = 1, #AIs do
				
				self:SendCards(AIs[n])
			end
		elseif world:EntityHasComponent(entities[i], "NewRound") then
			
			self:ResetValues()
		end
		
	end
end