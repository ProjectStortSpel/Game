AutoPickCards = System()

AutoPickCards.Initialize = function(self)
	--	Set Name
	self:SetName("AutoPickCards")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("DealtCard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AutoPickCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealingSettings", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("HasSelectedCards", FilterType.Excluded)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
end


AutoPickCards.StealCardsFrom = function(self, playerIndex)
	
	--	Counter for the actual selected cards
	local	nSelectedCards		=	0
	local	pickedCards			=	{}
			pickedCards.__mode	=	"k"
	
	--	Get the players information
	local	playerUnit		=	world:GetComponent(playerIndex, "UnitEntityId", "Id"):GetInt()
	local 	playerIp 		= 	world:GetComponent(playerIndex, "NetConnection", "IpAddress"):GetText()
	local	playerPort		= 	world:GetComponent(playerIndex, "NetConnection", "Port"):GetInt()
	
	--	First pick the cards that the player
	--	has selected (client wise)
	local	playerSelectedCards	=	self:GetEntities("ServerSelectedCard")
	local	currentIndexToPick	=	1
	while true do
		local	indexFound	=	false
		for cardIndex = 1, #playerSelectedCards do
			local cardOwner	= 	world:GetComponent(playerSelectedCards[cardIndex], "DealtCard", "PlayerEntityId"):GetInt()
			if cardOwner == playerIndex then
				local	tempIndex	=	world:GetComponent(playerSelectedCards[cardIndex], "ServerSelectedCard", "Index"):GetInt()
				
				--	Check if the current index is equal
				--	to the index we are currently looking for
				if currentIndexToPick == tempIndex then
					--	Add the index to the list
					pickedCards[#pickedCards+1]	=	playerSelectedCards[cardIndex]
					--	Update counters
					currentIndexToPick			=	currentIndexToPick+1
					nSelectedCards				=	nSelectedCards+1
					--	Set flag
					indexFound	=	true
					break
				end
			end
		end
		
		--	Break if we didn't find the index (not enough selected cards)
		if indexFound == false then
			break
		end
	end
	
	
	print("DealingSettings AutoPickCards")
	local DealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
	print("DealingSettings AutoPickCards done")
	
	--	Now pick the remaining cards from the 
	--	"non-selected" cards
	if nSelectedCards < cardsToPick then
		print("Start to pick from non-selected!")
		local	remainingCards 	= 	self:GetEntities("DealtCard")
		for i = 1, #remainingCards do
			
			--	Only check cards that doesn't have the flag
			if not world:EntityHasComponent(remainingCards[i], "ServerSelectedCard") then
			
				local cardOwner	= 	world:GetComponent(remainingCards[i], "DealtCard", "PlayerEntityId"):GetInt()
				if cardOwner == playerIndex then
					pickedCards[#pickedCards+1]	=	remainingCards[i]
					--	Update counters
					nSelectedCards				=	nSelectedCards+1
					
					if nSelectedCards == cardsToPick then
						break
					end
				end
			end
		end
	end
	
	print("Cards to kill: " .. #pickedCards)
	
	--	Kill all picked cards
	for	cardIndex = 1, #pickedCards do
	
		local action = world:GetComponent(pickedCards[cardIndex], "CardAction", "Action"):GetText()
		local prio = world:GetComponent(pickedCards[cardIndex], "CardPrio", "Prio"):GetInt()
		print("CardID: " .. pickedCards[cardIndex])
		print("Index: " .. cardIndex)
		print("Action: " .. action)
		print("Prio: " .. prio)
	
		world:RemoveComponentFrom("DealtCard", pickedCards[cardIndex])
		
		if world:EntityHasComponent(pickedCards[cardIndex], "ServerSelectedCard") then
			world:RemoveComponentFrom("ServerSelectedCard", pickedCards[cardIndex])
		end
		world:CreateComponentAndAddTo("CardStep", pickedCards[cardIndex])
		world:GetComponent(pickedCards[cardIndex], "CardStep", "Step"):SetInt(cardIndex)
		world:GetComponent(pickedCards[cardIndex], "CardStep", "UnitEntityId"):SetInt(playerUnit)
		
		Net.SendEntityKill(pickedCards[cardIndex], playerIp, playerPort)
	end
	
	--	Update unit status
	world:CreateComponentAndAddTo("UnitSelectedCards", playerUnit)
end

AutoPickCards.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "AutoPickCards") then
		
			print("Time to steal cards!")
			
			local nonReadyPlayers = self:GetEntities("Player")
			
			for tPlayer = 1, #nonReadyPlayers do
				self:StealCardsFrom(nonReadyPlayers[tPlayer])
			end
			
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("NotifyStartNewRound", id)
			
			world:KillEntity(entity)
		end
	end
end

Net.Receive("Server.HasSelectedCard",
	function( id, ip, port )	
		
		local	cardID		=	Net.ReadInt(id)
		local	cardIndex	=	Net.ReadInt(id)
		
		--	Add ServerSelectedCard to the card
		if not world:EntityHasComponent(cardID, "ServerSelectedCard") then
			world:CreateComponentAndAddTo("ServerSelectedCard", cardID)
			world:GetComponent(cardID, "ServerSelectedCard", "Index"):SetInt(cardIndex)
		end
	end
)

Net.Receive("Server.HasUnselectedCard",
	function( id, ip, port )	

		local	cardID		=	Net.ReadInt(id)
		local	cardIndex	=	Net.ReadInt(id)
		
		--	Remove ServerSelectedCard to the card
		if world:EntityHasComponent(cardID, "ServerSelectedCard") then
			world:RemoveComponentFrom("ServerSelectedCard", cardID)
		end
	end
)
