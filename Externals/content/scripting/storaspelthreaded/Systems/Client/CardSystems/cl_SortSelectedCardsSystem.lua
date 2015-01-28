SortSelectedCardsSystem = System()

SortSelectedCardsSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("SortSelectedCardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("SelectCard", FilterType.Mandatory)
end


SortSelectedCardsSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		local cards = self:GetEntities()
		if #cards <= 5 then
			local index = 1
			for i = 1, #cards do
				
				if world:EntityHasComponent(cards[i], "CardSelected") then
					index = index + 1
				end

			end

			world:SetComponent(entityId, "SelectCard", "Index", index)
			world:CreateComponentAndAddTo("CardSelected", entityId)
		else
			world:RemoveComponentFrom("SelectCard", entityId)
		end
	end
end


SortSelectedCardsSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, "CardSelected") then
			
			local index = world:GetComponent(entityId, "SelectCard", "Index"):GetInt()

			local cards = self:GetEntities()

			for i = 1, #cards do
				local index2 = world:GetComponent(cards[i], "SelectCard", "Index"):GetInt()

				if index2 > index then
					world:SetComponent(cards[i], "SelectCard", "Index", index2 - 1)
				end

			end
			
			world:RemoveComponentFrom("CardSelected", entityId)
		end
	end
end