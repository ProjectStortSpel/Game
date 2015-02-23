AICardPickingSystem = System()
AICardPickingSystem.PrintSimulation = 1
AICardPickingSystem.AICheat = 1
AICardPickingSystem.CardsToSimulate = 3
--AICardPickingSystem.PermutationsDone = false
--AICardPickingSystem.PermutationsArray = ""

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
end

AICardPickingSystem.Update = function(self, dt)
	
	local AIs = self:GetEntities("AI")
	local Cards = self:GetEntities("AICard")
	local DealingSettings = self:GetEntities("DealingSettings")
	--local PickedCards = {}
	--PickedCards.__mode = "k"
	
	
	
	-- TODO: Add some entity that will tell that the ai should work now.
	
	if #DealingSettings == 0 or #AIs == 0 then
		return
	end
	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
	
	if #Cards >= #AIs * cardsPerHand then
	
		local CPtiles = self:GetEntities("Checkpoint")
		
		for i = 1, #AIs do
			
			PotentialFieldHandler.UseMyPF(i)
						
			local unitID = world:GetComponent(AIs[i], "UnitEntityId", 0):GetInt()
			
			local cpTargetNr = world:GetComponent(unitID, "TargetCheckpoint", 0):GetInt()
			
			-- vart AIn är
			local aiPositionX, aiPositionY = world:GetComponent(unitID, "MapPosition", 0):GetInt2()
			-- AIs direction
			local aiDirX, aiDirY = world:GetComponent(unitID, "Direction", 0):GetInt2()
			local fellDown
			--Fetch the cards which is relevant to the current AI
			local CardSetAI = self:GetAIsCardSet(AIs[i], Cards)
			
			if #CardSetAI == cardsPerHand then
				--This will catch the best 
				local PickedCards = self:AIPickCards(CardSetAI, aiDirX, aiDirY, aiPositionX, aiPositionY, unitID)
				fellDown, aiPositionX, aiPositionY, aiDirX, aiDirY = self:SimulatePlayOfCards(unitID, PickedCards)
   
				if #PickedCards == cardsToPick then	
					
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

AICardPickingSystem.AIPickCards = function( self, CardSetAI, _dirX, _dirY, _posX, _posY, _unitID )
	
	local pickedCards = {}
	pickedCards.__mode = "k"
	
	local DealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
		
	if #CardSetAI >= cardsPerHand then
		
		if self.AICheat == 1 then
			self:ChangeTheCards(cardsPerHand, CardSetAI)
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
		
		
		
		
		
		--local forwards = self:GetAllCardsOf(CardSetAI, "Forward")
		--local backwards = self:GetAllCardsOf(CardSetAI, "Backward")
		--local turnLefts = self:GetAllCardsOf(CardSetAI, "TurnLeft")
		--local turnRights = self:GetAllCardsOf(CardSetAI, "TurnRight")
		--local turnArounds = self:GetAllCardsOf(CardSetAI, "TurnAround")
		--local sprints = self:GetAllCardsOf(CardSetAI, "Sprint")
		--local shots = self:GetAllCardsOf(CardSetAI, "SlingShot")
		
		local simFellDown, simPosX, simPosY, simDirX, simDirY
		local cardsToSim = {}
		cardsToSim.__mode = "k"
		local bestCardIds = {}
		bestCardIds.__mode = "k"
		local bestCardId, bestDist, bestNextDist
		local dist, nextDist
		
		local posX, posY, dirX, dirY = _posX, _posY, _dirX, _dirY
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
			
			-- If there is only one card left we don't need to get all permutations.
			--if noToGet == 1 then
				
				-- Use old 
				
				--for n = 1, cardsLeftInHand do
				--	charArray = charArray .. string.char(n)
				--end
			--else
				charArray = CombinationMath.Permutations(cardsLeftInHand, noToGet)
			--end
			
			-- Init variables to speed up usage of permutations.
			local lengthArray = string.len(charArray)
			local cardsToSimMinusOne = noToGet - 1
			
			for n = 1, noToGet do
				table.insert(cardsToSim, CardSetAI[n])
			end
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
			local cardIds = {}
			cardIds.__mode = "k"
			
			for permutationIndex = 1, lengthArray, noToGet do
				
				for k in pairs (cardIds) do
					cardIds[k] = nil
				end
				
				for cardIndex = 0, cardsToSimMinusOne do
					local charVar = string.byte(charArray, permutationIndex + cardIndex)
					table.insert(cardsToSim, CardSetAI[charVar])
					
					--cardIds = nil
					--cardIds = {}
			--print("haj3")
					table.insert(cardIds, charVar)
			--print("haj4")
					
					if cardIndex == 0 then
						firstCardId = charVar
					end
				end
				
				--print("haj", _unitID, posX, posY, dirX, dirY, targetX, targetY, #cardsToSim, cardsLeftToPick)
				io.write(_unitID, " ", posX, " ", posY, " ", dirX, " ", dirY, " ", targetX, " ", targetY, " ", #cardsToSim, " ", cardsLeftToPick, "\n")
				
				dist, simFellDown, simPosX, simPosY, simDirX, simDirY = self:GetSimDist(_unitID, posX, posY, dirX, dirY, targetX, targetY, cardsToSim, cardsLeftToPick)
				
				print("haj", permutationIndex, i)
				
				if dist <= bestDist then
					
					-- TODO: Only do nextDist(forward) if have forward cards, only do nextDist(backward) if have backward cards.
					-- TODO: Don't use walk-cards if we end up at the same spot.
					
					nextDist = self:GetSimNextDist(simFellDown, simPosX, simPosY, simDirX, simDirY, targetX, targetY, cardsLeftToPick)
										
					-- If the distance is the same as the best and the "nextDist" is better, then a better card has been found.
					if dist == bestDist and nextDist < bestNextDist then
						
						bestCardId = firstCardId
						bestNextDist = nextDist
						
						bestCardIds = cardIds
						
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
						
						bestCardIds = cardIds
						
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
			
			if (cardsLeftToPick == noOfCardsToSim) then
				
				print("Rest of cards picked", #bestCardIds)
				for n = 1, #bestCardIds do
					table.insert(pickedCards, CardSetAI[bestCardIds[n]])
					--table.remove(CardSetAI, bestCardId)
				end
				print("Rest of cards picked", #bestCardIds)
				
				break
			else
				print("card picked")
				-- Choose card, and simulate the choosen card to use the positions to choose the next card.
				table.insert(pickedCards, CardSetAI[bestCardId])
				table.insert(cardsToSim, CardSetAI[bestCardId])
				fellDown, posX, posY, dirX, dirY = self:SimulateCardsFromPos(_unitID, posX, posY, dirX, dirY, cardsToSim)
				table.remove(cardsToSim, 1)
				table.remove(CardSetAI, bestCardId)
				print("card picked")
			end
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
	
	local DealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)

	for i = 1, cardsToPick do
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
	
	local cardName = ""
	
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
					fellDown, posX, posY = self:SimulateMoveForwardRiver(posX, posY, waterDirX, waterDirY, true, false, 1, true)
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

AICardPickingSystem.SimulateMoveForwardRiver = function(self, _posX, _posY, _dirX, _dirY, _forwards, _jump, _iterations, _riverMove)
	
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
	
	print("ts")
	
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
		print("adding extraCost")
		local extraCost = _cardsLeftToPick + 1
		print("added extraCost", extraCost)
		return extraCost
	else
		return 0
	end
end

AICardPickingSystem.DoPermutations = function(self)
	
	local DealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
	local charArray = CombinationMath.Permutations(cardsPerHand, self.CardsToSimulate)
	
	--for i = 0, cardsToPick - 2 do
	--	print(cardsPerHand - i, self.CardsToSimulate - i)
	--	charArray = CombinationMath.Permutations(cardsPerHand - i, self.CardsToSimulate - i)
	--end
	
	--for i = 0, 5 do
	--	for n = 1 
	--	print(string.byte(charArray, i))
	--end
	
	--print(#charArray)
	
	--for permutationIndex = 1, #charArray, 5 do
	--	for cardIndex = 0, 4 do
	--		print(string.byte(charArray, permutationIndex + cardIndex))
	--	end
	--end
	
	self.PermutationsArray = charArray
end

AICardPickingSystem.ChangeTheCards = function(self, _cardsPerHand, _cardSet)
	
	for j = 1, _cardsPerHand do
		
		local cardactioncomp = world:GetComponent(_cardSet[j], "CardAction", 0)
		
			if j == 1 then cardactioncomp:SetText("Forward")
		elseif j == 2 then cardactioncomp:SetText("Forward")
		elseif j == 3 then cardactioncomp:SetText("Backward")
		elseif j == 4 then cardactioncomp:SetText("TurnRight")
		elseif j == 5 then cardactioncomp:SetText("TurnRight")
		elseif j == 6 then cardactioncomp:SetText("TurnLeft")
		elseif j == 7 then cardactioncomp:SetText("TurnAround")
		elseif j == 8 then cardactioncomp:SetText("TurnAround")
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

-- Simulate the playing of the nth card and return the pathfinder's value from that position to the target.
AICardPickingSystem.GetSimDist = function(self, _unitID, _posX, _posY, _dirX, _dirY, _targetX, _targetY, _cardsToSim, _cardsLeftToPick)
	print("rstine")
	local simFellDown, simPosX, simPosY, simDirX, simDirY = self:SimulateCardsFromPos(_unitID, _posX, _posY, _dirX, _dirY, _cardsToSim)
	
	print("rstine1.2")
	local dist = PathfinderHandler.GeneratePath(simPosX, simPosY, _targetX, _targetY)
	
	print("rstine1.5")
	-- Add extraCost if the AI will fall down by playing this card.
	dist = dist + self:AddExtraCostIfFellDown(simFellDown, _cardsLeftToPick)
	
	print("rstine2")
	return dist, simFellDown, simPosX, simPosY, simDirX, simDirY
end

AICardPickingSystem.GetSimNextDist = function(self, _simFellDown, _simPosX, _simPosY, _simDirX, _simDirY, _targetX, _targetY, _cardsLeftToPick)
	
	-- Get the distance from a cell as if we have walked forward, then get distance as if we walked forward and compare the two. Save the best.
	local nextDist = PathfinderHandler.GeneratePath(_simPosX + _simDirX, _simPosY + _simDirY, _targetX, _targetY)
	nextDist = math.min(nextDist, PathfinderHandler.GeneratePath(_simPosX - _simDirX, _simPosY - _simDirY, _targetX, _targetY))
	
	-- Add extraCost if the AI fell down by playing the card.
	nextDist = nextDist + self:AddExtraCostIfFellDown(_simFellDown, _cardsLeftToPick)
	
	return nextDist
end

AICardPickingSystem.EntitiesAdded = function(self, dt, entities)

	for i = 1, #entities do
		if world:EntityHasComponent(entities[i], "DealtCard") then
			local playerid = world:GetComponent(entities[i], "DealtCard", 0)
			local id = playerid:GetInt()
			local plynum = world:GetComponent(id, "PlayerNumber", 0):GetInt()
			local card = world:GetComponent(entities[i], "CardAction", 0):GetText()
		-- If an AI was added, do permutations if it is the first AI.
		--elseif world:EntityHasComponent(entities[i], "AI") and not self.PermutationsDone then
		--	self:DoPermutations()
		--	self.PermutationsDone = true
		end
	end
end