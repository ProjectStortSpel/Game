SortClientSelectedCards = System()

SortClientSelectedCards.Initialize = function(self)
	--	Set Name
	self:SetName("AutoPickCards")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesRemoved()
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("ServerSelectedCard", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("HasSelectedCards", FilterType.Excluded)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
end

SortClientSelectedCards.PrintCard = function(self, card)
	local 	action = world:GetComponent(card, "CardAction", "Action"):GetText()
	local 	prio = world:GetComponent(card, "CardPrio", "Prio"):GetInt()
	local	tempIndex	=	world:GetComponent(card, "ServerSelectedCard", "Index"):GetInt()
	print("--	Index " .. tempIndex .. "	--")
	print("Action:	" .. action)
	print("Prio:	" .. prio)
end

SortClientSelectedCards.GetPlayerSelectedCards = function(self, playerId)
	
	local	tempCards		=	{}
			tempCards.__mode = "k"
			
	local	allCards	=	self:GetEntities()
	for i = 1, #allCards do
		local	tempOwner	=	world:GetComponent(allCards[i], "DealtCard", "PlayerEntityId"):GetInt()
		if tempOwner == playerId then
			tempCards[#tempCards+1]	=	allCards[i]
		end
	end
	
	return tempCards;
end

SortClientSelectedCards.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		local 	currentOwner	=	world:GetComponent(entity, "DealtCard", "PlayerEntityId"):GetInt()
		local	currentIndex	=	world:GetComponent(entity, "ServerSelectedCard", "Index"):GetInt()
		
		local	ownerCards	=	self:GetPlayerSelectedCards(currentOwner)
		
		for i = 1, #ownerCards do
		
			if ownerCards[i] ~= entity then
				local	tempIndex	=	world:GetComponent(ownerCards[i], "ServerSelectedCard", "Index"):GetInt()
				
				if tempIndex >= currentIndex then
					world:GetComponent(ownerCards[i], "ServerSelectedCard", "Index"):SetInt(tempIndex+1)
				end
			end
			self:PrintCard(ownerCards[i])
		end
	end
end
SortClientSelectedCards.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		local 	currentOwner	=	world:GetComponent(entity, "DealtCard", "PlayerEntityId"):GetInt()
		local	ownerCards	=	self:GetPlayerSelectedCards(currentOwner)
		local	index		=	world:GetComponent(entity, "ServerSelectedCard", "Index"):GetInt()
		
		for i = 1, #ownerCards do
			local index2 = world:GetComponent(ownerCards[i], "ServerSelectedCard", "Index"):GetInt()
			if index2 > index then
				world:GetComponent(ownerCards[i], "ServerSelectedCard", "Index"):SetInt(index2-1)
			end
			
			self:PrintCard(ownerCards[i])
		end
		
	end
end