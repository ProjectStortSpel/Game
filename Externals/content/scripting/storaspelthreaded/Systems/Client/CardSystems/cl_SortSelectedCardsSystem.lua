SortSelectedCardsSystem = System()

SortSelectedCardsSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("SortSelectedCardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("SelectCard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealingSettings", FilterType.RequiresOneOf)
end


SortSelectedCardsSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, "SelectCard") then
			self:SelectCard(entityId)
		end
	end
end

SortSelectedCardsSystem.EntitiesRemoved = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, "CardSelected") then
			self:DeselectCard(entityId)
		end
	end
end

SortSelectedCardsSystem.SelectCard = function(self, card)
	local index = world:GetComponent(card, "SelectCard", "Index"):GetInt()
	local cards = self:GetEntities("SelectCard")
	if index > #cards then
		index = #cards
	end
	
	local DealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
	
	if #cards <= cardsToPick then
		for i = 1, #cards do
			if world:EntityHasComponent(cards[i], "CardSelected") then
				local index2 = world:GetComponent(cards[i], "SelectCard", "Index"):GetInt()
				if index2 >= index then
					world:SetComponent(cards[i], "SelectCard", "Index", index2 + 1)
				end
			end
		end
		
		world:SetComponent(card, "SelectCard", "Index", index)
		world:CreateComponentAndAddTo("CardSelected", card)
		self:SendSelectedCard(card, index)
	else
		world:RemoveComponentFrom("SelectCard", card)
		self:SendUnselectedCard(card, index)
	end
	
end

SortSelectedCardsSystem.DeselectCard = function(self, card)
	local index = world:GetComponent(card, "SelectCard", "Index"):GetInt()
	local cards = self:GetEntities()
	for i = 1, #cards do
		local index2 = world:GetComponent(cards[i], "SelectCard", "Index"):GetInt()
		if index2 > index then
			world:SetComponent(cards[i], "SelectCard", "Index", index2 - 1)
		end
	end
	world:RemoveComponentFrom("CardSelected", card)
	self:SendUnselectedCard(card, index)
end

SortSelectedCardsSystem.SendSelectedCard = function(self, card, index)
	local pickingStartedID = Net.StartPack("Server.HasSelectedCard")
	Net.WriteInt(pickingStartedID, Net.ToServerID(card))
	Net.WriteInt(pickingStartedID, index)
	Net.SendToServer(pickingStartedID)
end

SortSelectedCardsSystem.SendUnselectedCard = function(self, card, index)
	local pickingStartedID = Net.StartPack("Server.HasUnselectedCard")
	Net.WriteInt(pickingStartedID, Net.ToServerID(card))
	Net.WriteInt(pickingStartedID, index)
	Net.SendToServer(pickingStartedID)
end



