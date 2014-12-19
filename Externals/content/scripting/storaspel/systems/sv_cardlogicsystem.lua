
DealCards = false


CreateDeckSystem = System()

CreateDeckSystem.GameRunning = false


CreateDeckSystem.Initialize = function ( self )
	self:SetName("CreateDeckSystem")
	
	self:AddComponentTypeToFilter("Unit", FilterType.Mandatory)

	print("CreateDeckSystem Initialized")
end

CreateDeckSystem.Update = function( self, dt )
	
	if GameRunning ~= CreateDeckSystem.GameRunning then
		
		CreateDeckSystem.GameRunning = GameRunning

		if GameRunning then 
			self:CreateDeck()
			DealCards = true
		end
	end

end

CreateDeckSystem.CreateDeck = function (self)
	
	local prio = 0
	
	local mySeed = os.time() - 1418742000 -- dont ask
	
	math.randomseed(mySeed)
	for i = 1, 100 * #self:GetEntities() do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		
		local rand = math.random(1, 90)
		
		if rand < 30 then
			cardactioncomp:SetString("Forward")
		elseif rand < 50 then
			cardactioncomp:SetString("Backward")
		elseif rand < 65 then
			cardactioncomp:SetString("TurnRight")
		elseif rand < 80 then
			cardactioncomp:SetString("TurnLeft")
		elseif rand < 90 then
			cardactioncomp:SetString("TurnAround")
		else
			cardactioncomp:SetString("Forward")
			
		end
	end
end

DealCardSystem = System()
DealCardSystem.DealCard = false
DealCardSystem.FirstDeal = true

DealCardSystem.Initialize = function ( self )
	self:SetName("DealCardSystem")
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardAction", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UsedCard", FilterType.Excluded)
	self:AddComponentTypeToFilter("DealtCard", FilterType.Excluded)

	print("DealCardSystem Initialized")
end

DealCardSystem.Update = function( self, dt )

	if self.DealCard then

		if self.FirstDeal then			
			self:DealCards(8)	
			self.FirstDeal = false
		else	
			self:DealCards(5)
		end
		self.DealCard = false
	end

	if DealCards then
		
		self.DealCard = true

		DealCards = false
	end

end

DealCardSystem.DealCards = function (self, numCards)

	local players = self:GetPlayers()
	local cards = self:GetCards()
	print("DealCards")
	print("Numplayer: " .. #players)
	print("NumCards: " .. #cards)
	print("")
	for i = 1, #players do
		
		local ip = world:GetComponent(players[i], "NetConnection", "IpAddress"):GetString()
		local port = world:GetComponent(players[i], "NetConnection", "Port"):GetInt()

		
		print("IP:   " .. ip)
		print("Port: " .. port)
		print("NumCards:" .. numCards)
		print("")
		for j = 1, numCards do
			local card = cards[(i - 1) * #players + j]
			world:CreateComponentAndAddTo("DealtCard", card)
			world:SetComponent(card, "DealtCard", "PlayerEntityId", players[i])
			Net.SendEntity(card, ip, port)	
		end
		
		Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
	end

end

DealCardSystem.GetCards = function (self)
	
	local cards = {}
	local index = 1;
	local entities = self:GetEntities()
	for i = 1, #entities do
		if self:EntityHasComponent(entities[i], "CardAction") then
			cards[index] = entities[i]
			index = index + 1
		end
	end
	return cards
end

DealCardSystem.GetPlayers = function (self)
	
	local players = {}
	local index = 1;
	local entities = self:GetEntities()
	for i = 1, #entities do
		if self:EntityHasComponent(entities[i], "Player") then
			players[index] = entities[i]
			index = index + 1
		end
	end
	return players
end

Net.Receive("Server.SelectCards", 
	function( id, ip, port )	

		print("Client selected cards:")
		print("")
		for i = 1, 5 do

			local card = Net.ReadInt(id)
			print("EntID: " .. card)
			if  DealCardSystem:EntityHasComponent(card, "DealtCard") then
			
				local player = world:GetComponent(card, "DealtCard", "PlayerEntityId"):GetInt()
				local playerIp = world:GetComponent(player, "NetConnection", "IpAddress"):GetString()
				local playerPort = world:GetComponent(player, "NetConnection", "Port"):GetInt()

				if playerIp == ip and playerPort == port then
					world:RemoveComponentFrom("DealtCard", card)
					world:CreateComponentAndAddTo("CardStep", card)
					world:SetComponent(card, "CardStep", "Step", i)
					local unit = world:GetComponent(player, "UnitEntityId", "Id"):GetInt()
					world:SetComponent(card, "CardStep", "UnitEntityId", unit)
				else
					Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
					return
				end

			else
				Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
				return
			end

		end	
	end
)


--CardDeckSystem.GetStartingCards = function (self, nrOfCardsPerPlayer, nrOfPlayers)
	--
	--self.NR_OF_CARDS_PER_PLAYER = nrOfCardsPerPlayer
	--self.NR_OF_PLAYERS = nrOfPlayers
	--
	--local noOfCards = self.NR_OF_CARDS_PER_PLAYER * self.NR_OF_PLAYERS
	--
	--for i = 1, noOfCards do
		--table.insert(self.DealtCards, -1)
	--end
	--
	--
	--local cards = {}
	--
	--for i = 1, noOfCards do
		--local card = self.CardDeck[1]
		--cards[i] = card
		----local index = i + (playerNumber - 1) * self.NR_OF_CARDS_PER_PLAYER
		----print("Inserting in ", index) 
		----self.DealtCards[index] = card
		--self.DealtCards[i] = card
		----table.insert(self.DealtCards, index, card)
		----print("Success")
		--table.remove(self.CardDeck, 1)
		--
	--end
	--
	--for i = 1, noOfCards do
		--
		--local cardactioncomp = self:GetComponent( self.DealtCards[i], "CardAction", 0)
		--local cardaction = cardactioncomp:GetString()
		--print(i, cardaction, self.DealtCards[i])
	--end
	--
	--return cards
--end



--
--CardDeckSystem.PlayerChoseCards = function(self, cardIds, playerNumber)
	--
	--local offset = (playerNumber - 1) * self.NR_OF_CARDS_PER_PLAYER
	--
	--for i = 1, #cardIds do
	--
		--index = cardIds[i] + offset
		----print(index, #self.DealtCards)
		--
		--table.insert(self.UsedCards, self.DealtCards[index])
		--table.remove(self.DealtCards, index)
		--
		--local newCard = self:GetNewCard()
		--table.insert(self.DealtCards, newCard)
		--
	--end
		--
--end
--
--CardDeckSystem.GetNewCard = function(self)
--
	--if #self.CardDeck < 1 then
		--self:Reshuffle()
	--end
	--
	--local comp = self:GetComponent(self.CardDeck[1], "CardAction", 0)
	--local stringData = comp:GetString()
	--
	--print(self.CardDeck[1], "Get New Card, ", stringData,". Cards left: ", #self.CardDeck)
	--
	--local newCard = self.CardDeck[1]
	--table.remove(self.CardDeck, 1)
	----table.insert(self.DealtCards, newCard)
	--
	--return newCard
--end
--
--CardDeckSystem.Reshuffle = function (self)
	--
	--while #self.UsedCards > 0 do		
		--local card = self.UsedCards[#self.UsedCards]
		--table.remove(self.UsedCards, #self.UsedCards)
		--table.insert(self.CardDeck, card)
	--end
--
	----while #self.DealtCards > 0 do
		----
		----table.insert(self.CardDeck, 
	----end
--end

----GenCarDeck = function(  )
----	local prio = 0
----	-- forward
----	for i = 1, 10*NROFPLAYERS do
----		prio = prio + 1
----		self:GetComponent(entity, "Position", 0)
----		local entity = world:CreateNewEntity()
----		local entity = world:CreateNewEntity("Player")
----		world:CreateComponentAndAddTo("Spawn", entity)
----		world:CreateComponentAndAddTo("SyncNetwork", entity)
----		local mapPos = {i,12}
----		--local mapPos = {i, 6}
----		self:SetPosition(entity, mapPos[1], 1.0, mapPos[2])
----		local comp = self:GetComponent(entity, "Spawn", 0)
----		comp:SetInt2(mapPos[1], mapPos[2])
----		
----	end
----	
----	--backward
----	
----	--turnright
----	
----	--turnleft
----
----end
--
--
--
--
--
------------------------------ ForwardSystem
--
--ForwardSystem = System()
--
--ForwardSystem.Initialize = function(self)
	--self:SetName("Forward System")
	--self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Forward",FilterType.Mandatory)
	--
	--self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
	--print("ForwardSystem initialized!")
--end
--
--ForwardSystem.OnEntityAdded = function(self, entity)
	--local dir = self:GetComponent(entity, "Direction", 0)
	--local mapPos = self:GetComponent(entity, "MapPosition", 0)
	--
	--local x, y = mapPos:GetInt2()
	--local dirX, dirY = dir:GetInt2()
	--
	--local targetX = x + dirX
	--local targetY = y + dirY
	--
	--local moved = PlayerMovementSystem:MoveTo(entity, targetX, targetY, dirX, dirY)
	--
	--world:RemoveComponentFrom("Forward", entity);
--end
--
--
------------------------------ BackwardSystem
--
--BackwardSystem = System()
--
--BackwardSystem.Initialize = function(self)
	--self:SetName("Backward System")
	--self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Backward",FilterType.Mandatory)
	--
	--self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
	--print("BackwardSystem initialized!")
--end
--
--BackwardSystem.OnEntityAdded = function(self, entity)
	--local dir = self:GetComponent(entity, "Direction", 0)
	--local mapPos = self:GetComponent(entity, "MapPosition", 0)
	--
	--local x, y = mapPos:GetInt2()
	--local dirX, dirY = dir:GetInt2()
	--
	--local targetX = x - dirX
	--local targetY = y - dirY
	--
	--local moved = PlayerMovementSystem:MoveTo(entity, targetX, targetY, -dirX, -dirY)
	--
	--world:RemoveComponentFrom("Backward", entity);
--end
--
------------------------------ RightTurnSystem
--RightTurnSystem = System()
--
--RightTurnSystem.Initialize = function(self)
	--self:SetName("Right Turn System")
	--self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	--self:AddComponentTypeToFilter("TurnRight",FilterType.Mandatory)
	--
	--print("RightTurnSystem initialized!")
--end
--
--RightTurnSystem.OnEntityAdded = function(self, entity)
	--local dir = self:GetComponent(entity, "Direction", 0)
	--local rot = self:GetComponent(entity, "Rotation", 4)
	--local dx, dy = dir:GetInt2()
	--local roty = rot:GetFloat()
	--
	--local tempdy = dx
	--dx = -dy
	--dy = tempdy
	--
	--dir:SetInt2(dx, dy)
	--rot:SetFloat(roty - math.pi/2)
	--
	--world:RemoveComponentFrom("TurnRight", entity);
--end
--
------------------------------ LeftTurnSystem
--
--LeftTurnSystem = System()
--
--LeftTurnSystem.Initialize = function(self)
	--self:SetName("Left Turn System")
	--self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	--self:AddComponentTypeToFilter("TurnLeft",FilterType.Mandatory)
	--
	--print("LeftTurnSystem initialized!")
--end
--
--LeftTurnSystem.OnEntityAdded = function(self, entity)
	--local dir = self:GetComponent(entity, "Direction", 0)
	--local rot = self:GetComponent(entity, "Rotation", 4)
	--local dx, dy = dir:GetInt2()
	--local roty = rot:GetFloat()
	--local tempdy = dx
	--dx = dy
	--dy = -tempdy
	--
	--dir:SetInt2(dx, dy)
	--rot:SetFloat(roty + math.pi/2)
	--
	--world:RemoveComponentFrom("TurnLeft", entity);
--end
--
------------------------------ TurnAroundSystem
--
--TurnAroundSystem = System()
--
--TurnAroundSystem.Initialize = function(self)
	--self:SetName("Turn Around System")
	--self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	--self:AddComponentTypeToFilter("TurnAround",FilterType.Mandatory)
	--
	--print("TurnAroundSystem initialized!")
--end
--
--TurnAroundSystem.OnEntityAdded = function(self, entity)
	--local dir = self:GetComponent(entity, "Direction", 0)
	--local rot = self:GetComponent(entity, "Rotation", 4)
	--local dx, dy = dir:GetInt2()
	--local roty = rot:GetFloat()
	--
	--dx = -dx
	--dy = -dy
	--
	--dir:SetInt2(dx, dy)
	--rot:SetFloat(roty + math.pi)
	--
	--world:RemoveComponentFrom("TurnAround", entity);
--end
--
--
--