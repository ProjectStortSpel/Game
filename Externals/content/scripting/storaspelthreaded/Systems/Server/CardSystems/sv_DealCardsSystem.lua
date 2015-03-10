DealCardsSystem = System()
DealCardsSystem.DealCard = false
DealCardsSystem.FirstDeal = true

DealCardsSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("DealCardsSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("DealCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardAction", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealingSettings", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UsedCard", FilterType.Excluded)
	self:AddComponentTypeToFilter("CardStep", FilterType.Excluded)
	self:AddComponentTypeToFilter("DealtCard", FilterType.Excluded)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
end

DealCardsSystem.ClearSelectedCards = function(self)
	
	local	selectedCards	=	self:GetEntities("ServerSelectedCard")
	for i = 1, #selectedCards do
		world:GetComponent(selectedCards[i], "ServerSelectedCard", "Index"):SetInt(0)
		world:RemoveComponentFrom("ServerSelectedCard", selectedCards[i])
	end
	
end

DealCardsSystem.EntitiesAdded = function(self, dt, entities)

	local dealCards = false

	for n = 1, #entities do
		local entityId = entities[n]

		if world:EntityHasComponent( entityId, "DealCards") then
			self:ClearSelectedCards()
			local numCards = world:GetComponent( entityId, "DealCards", "NumCards"):GetInt()
			self:DealCards(numCards)

			world:KillEntity( entityId )
			
			dealCards = true
		end
	end
	
	if dealCards then
		local audioId = Net.StartPack("Client.FadeInSoundC")
		Net.WriteString(audioId, "InGame")
		Net.WriteString(audioId, "PickingMusic")
		Net.WriteInt(audioId, 200)
		Net.WriteBool(audioId, false)
		Net.Broadcast(audioId)
	end
end

DealCardsSystem.DealCards = function (self, numCards)

	local players = self:GetEntities("Player")
	local aiPlayers = self:GetEntities("AI")
	local cards = self:GetEntities("Card")
	local cardsLeft = #cards
	--print("DealCards")
	--print("Numplayer: " .. #players)
	--print("NumCards: " .. cardsLeft)
	--print("NumCardsToDeal: " .. numCards)
	
	local Cards = { }
	Cards.__mode = "k"
	Cards.Move = self:GetEntities("CardTypeMove")
	Cards.Turn = self:GetEntities("CardTypeTurn")
	Cards.Ability = self:GetEntities("CardTypeAbility")
	Cards.MoveLeft = #Cards.Move
	Cards.TurnLeft = #Cards.Turn
	Cards.AbilityLeft = #Cards.Ability

	--print("")
	
	for i = 1, #players do
		
		if world:EntityHasComponent(players[i], "HasSelectedCards") then
			world:RemoveComponentFrom("HasSelectedCards", players[i])
		end
	
		local ip = world:GetComponent(players[i], "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(players[i], "NetConnection", "Port"):GetInt()
		
		Cards.MoveCount, Cards.TurnCount, Cards.AbilityCount = self:CalculateCardTypeBalance(numCards, Cards.MoveLeft, Cards.TurnLeft, Cards.AbilityLeft)
		for j = 1, Cards.MoveCount do
			Cards.MoveLeft = self:DealCardToPlayer(Cards.Move, Cards.MoveLeft, players[i], ip, port)
		end
		for j = 1, Cards.TurnCount do
			Cards.TurnLeft = self:DealCardToPlayer(Cards.Turn, Cards.TurnLeft, players[i], ip, port)
		end
		for j = 1, Cards.AbilityCount do
			Cards.AbilityLeft = self:DealCardToPlayer(Cards.Ability, Cards.AbilityLeft, players[i], ip, port)
		end
		
		--Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
	end
	
	for i = 1, #aiPlayers do
		
		if world:EntityHasComponent(aiPlayers[i], "HasSelectedCards") then
			world:RemoveComponentFrom("HasSelectedCards", aiPlayers[i])
		end
		
		Cards.MoveCount, Cards.TurnCount, Cards.AbilityCount = self:CalculateCardTypeBalance(numCards, Cards.MoveLeft, Cards.TurnLeft, Cards.AbilityLeft)
		for j = 1, Cards.MoveCount do
			Cards.MoveLeft = self:DealCardToAI(Cards.Move, Cards.MoveLeft, aiPlayers[i])
		end
		for j = 1, Cards.TurnCount do
			Cards.TurnLeft = self:DealCardToAI(Cards.Turn, Cards.TurnLeft, aiPlayers[i])
		end
		for j = 1, Cards.AbilityCount do
			Cards.AbilityLeft = self:DealCardToAI(Cards.Ability, Cards.AbilityLeft, aiPlayers[i])
		end
	end
	
	--	Notify players about timer
	local newId = world:CreateNewEntity();
	world:CreateComponentAndAddTo("OnPickingPhase", newId);
end

DealCardsSystem.DealCardToPlayer = function(self, cards, cardsLeft, player, ip, port)
	local cardIndex = math.random(1, cardsLeft)
	local card = cards[cardIndex]
	
	world:CreateComponentAndAddTo("DealtCard", card)
	world:SetComponent(card, "DealtCard", "PlayerEntityId", player)
	
	local unit = world:GetComponent(player, "UnitEntityId", "Id"):GetInt(0)
	world:SetComponent(card, "Card", "Unit", unit)
	
	Net.SendEntity(card, ip, port)

	table.remove(cards, cardIndex)
	return cardsLeft - 1
end

DealCardsSystem.DealCardToAI = function(self, cards, cardsLeft, ai)
	local cardIndex = math.random(1, cardsLeft)
	local card = cards[cardIndex]
	
	world:CreateComponentAndAddTo("DealtCard", card)
	if not world:EntityHasComponent(card, "AICard") then
		world:CreateComponentAndAddTo("AICard", card)
	end
	world:SetComponent(card, "DealtCard", "PlayerEntityId", ai)
	
	table.remove(cards, cardIndex)
	return cardsLeft - 1
end

DealCardsSystem.CalculateCardTypeBalance = function(self, numCards, moveCardsLeft, turnCardsLeft, abilityCardsLeft)
	local CardCount = { }
	CardCount.__mode = "k"
	
	CardCount.Remaining = numCards
	
	----------------------------------------------------------------------------------------------
	-- The values below are (sort of) percentage-based. Change them to balance the cards dealt! --
	----------------------------------------------------------------------------------------------
	CardCount.MoveCardCountMin = math.ceil(0.3 * CardCount.Remaining) -- 3 / 2
	CardCount.MoveCardCountMax = math.ceil(0.5 * CardCount.Remaining) -- 4 / 3
	
	CardCount.TurnCardCountMin = math.ceil(0.2 * CardCount.Remaining) -- 3 / 2
	CardCount.TurnCardCountMax = math.ceil(0.4 * CardCount.Remaining) -- 4 / 3
	
	CardCount.AbilityCardCountMin = math.ceil(0.1 * CardCount.Remaining) -- 1 / 1
	CardCount.AbilityCardCountMax = math.ceil(0.3 * CardCount.Remaining) -- 2 / 2
	----------------------------------------------------------------------------------------------
	
	CardCount.MoveCardCount = math.random(CardCount.MoveCardCountMin, CardCount.MoveCardCountMax)
	
	CardCount.Remaining = CardCount.Remaining - CardCount.MoveCardCount
	if CardCount.Remaining < CardCount.TurnCardCountMax then
		CardCount.TurnCardCountMax = CardCount.Remaining
	end
	if CardCount.Remaining > CardCount.TurnCardCountMin + CardCount.AbilityCardCountMax then
		CardCount.TurnCardCountMin = CardCount.Remaining - CardCount.AbilityCardCountMax
	end
	
	CardCount.TurnCardCount = math.random(CardCount.TurnCardCountMin, CardCount.TurnCardCountMax)
	
	CardCount.Remaining = CardCount.Remaining - CardCount.TurnCardCount
	if CardCount.Remaining < CardCount.AbilityCardCountMax then
		CardCount.AbilityCardCountMax = CardCount.Remaining
	end
	if CardCount.Remaining > CardCount.AbilityCardCountMin then
		CardCount.AbilityCardCountMin = CardCount.Remaining
	end
	
	CardCount.AbilityCardCount = math.random(CardCount.AbilityCardCountMin, CardCount.AbilityCardCountMax)
	
	if CardCount.MoveCardCount > moveCardsLeft then
		CardCount.TurnCardCount = CardCount.TurnCardCount + CardCount.MoveCardCount - moveCardsLeft
		CardCount.MoveCardCount = moveCardsLeft
	end
	if CardCount.TurnCardCount > turnCardsLeft then
		CardCount.AbilityCardCount = CardCount.AbilityCardCount + CardCount.TurnCardCount - turnCardsLeft
		CardCount.TurnCardCount = turnCardsLeft
	end
	if CardCount.AbilityCardCount > abilityCardsLeft then
		CardCount.AbilityCardCount = abilityCardsLeft
	end
	
	return CardCount.MoveCardCount, CardCount.TurnCardCount, CardCount.AbilityCardCount
end

Net.Receive("Server.SelectCards",
	function( id, ip, port )
	
		--	Selected cards from player
		local	selectedCards			=	{}
				selectedCards.__mode	=	"k"
				
		--	Check all cards from the player
		local	currentPlayer, pIP, pPORT
		
		local DealingSettings = DealCardsSystem:GetEntities("DealingSettings")
		local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
		
		for i = 1, cardsToPick do
			
			--	Get the current card
			local	tempCard	=	Net.ReadInt(id)
			
			if world:EntityHasComponent(tempCard, "DealtCard") then
				currentPlayer	=	world:GetComponent( tempCard, "DealtCard", "PlayerEntityId"):GetInt()
				pIP 			= 	world:GetComponent( currentPlayer, "NetConnection", "IpAddress"):GetText()
				pPORT 			= 	world:GetComponent( currentPlayer, "NetConnection", "Port"):GetInt()
				
				--	Correct player
				if pIP == ip and pPORT == port then
					selectedCards[#selectedCards+1]	=	tempCard
				else
					print("TRYING TO PLAY A CARD THAT AINT THEIRS!!")
					Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
					return
				end	
				
				
			else
				print("TRYING TO PLAY A UNDEALT CARD! CHEATER")
				Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
				return
			end
		
		
		end
		
		--	All cards are legit
		--	start next phase
		
		--	Get the players unit
		local	pUnit	=	world:GetComponent(currentPlayer, "UnitEntityId", "Id"):GetInt()
		world:CreateComponentAndAddTo("HasSelectedCards", currentPlayer)
		
		for n = 1, cardsToPick do
			local	cAction	=	world:GetComponent(selectedCards[n], "CardAction", "Action"):GetText()
			local	cPrio	=	world:GetComponent(selectedCards[n], "CardPrio", "Prio"):GetInt()
			
			
			world:RemoveComponentFrom("DealtCard", selectedCards[n])
			world:CreateComponentAndAddTo("CardStep", selectedCards[n])
			
			world:GetComponent(selectedCards[n], "CardStep", "Step"):SetInt(n)
			world:GetComponent(selectedCards[n], "CardStep", "UnitEntityId"):SetInt(pUnit)
			
			Net.SendEntityKill(selectedCards[n], ip, port)
		end
		
		world:CreateComponentAndAddTo("UnitSelectedCards", pUnit)
		
		local	id	=	world:CreateNewEntity()
		world:CreateComponentAndAddTo("NotifyStartNewRound", id)
		
	end
)